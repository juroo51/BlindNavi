#include "stdafx.h"
#include "PlayFromFile.h"
#include "Rle.h"
#include "Sectors.h"

PlayFromFile::~PlayFromFile()
{
	input.close();
}

void PlayFromFile::run(std::string file)
{
	openFile(file);

	if (!input.is_open())
	{
		std::cout << "Unable to open file" << std::endl;
		system("pause");
		return;
	}

	getAtributes();

	std::vector<UINT16> compressedBuffer(widthOfFrame * heightOfFrame * 2);
	std::vector<UINT16> depthBuffer(widthOfFrame * heightOfFrame);
	std::vector<char> bigBuffer(widthOfFrame * heightOfFrame * frameRate * sizeof(UINT16) * 2);
	
	std::size_t frameStart;
	std::streampos filePos{ input.tellg() };
	std::streampos bigBufferBytesCount{ 0 };
	
	unsigned int frameCount{ 0 };
	bool escPressed{ false };

	while (true)
	{
		frameStart = 0;
		if (escPressed) { return; }
		bigBufferBytesCount = (input.read(bigBuffer.data(), bigBuffer.size())).gcount();
		if (bigBufferBytesCount < bigBuffer.size()) { return; }
		
		while (frameStart < bigBuffer.size() && !escPressed)
		{
			frameSize = *((std::size_t*)(bigBuffer.data() + frameStart));
			memcpy(compressedBuffer.data(), (bigBuffer.data() + frameStart + sizeof(std::size_t)), frameSize * sizeof(UINT16) + sizeof(std::size_t));
			
			if (compressedFile) { RleCompression::decompress(compressedBuffer, frameSize, depthBuffer); }
			else { memcpy(depthBuffer.data(), compressedBuffer.data(), frameSize * sizeof(UINT16)); }

			frameCount++;
			drawDepth(depthBuffer, frameCount);
			cv::imshow("BlindNavi", cvDepthMatConversed);
			cv::waitKey(60);

			if (frameStart + frameSize * sizeof(UINT16) + sizeof(std::size_t) > bigBuffer.size()) { break; }
			frameStart += frameSize * sizeof(UINT16) + sizeof(std::size_t);

			if (GetAsyncKeyState(VK_ESCAPE)) { escPressed = true; }
			
			pause();
			hold();
		}
		filePos += frameStart;
		input.seekg(filePos, std::ios::beg);
	}
}

void PlayFromFile::openFile(std::string file)
{
	std::string fileName;
	fileName = file;
	std::string path{ "C:/Users/jlipovsky/Desktop/TestVids/" };
	path += fileName;
	input.open(path.c_str(), std::ios::beg | std::fstream::binary);
}

void PlayFromFile::pause()
{
	bool pauseKey{ false };
	if (GetAsyncKeyState(VK_LSHIFT))
	{
		pauseKey = true;
		while (pauseKey)
		{
			if (GetAsyncKeyState(VK_RSHIFT)) { pauseKey = false; }
			Sleep(100);
		}
	}
}

void PlayFromFile::hold()
{
	bool pauseKey{ false };
	while (GetAsyncKeyState(VK_SPACE))
	{
		Sleep(100);
		continue;
	}
}

void PlayFromFile::getAtributes()
{
	char tmp[sizeof(unsigned int) * 2 + sizeof(char)];
	input.read((char*)&tmp, sizeof(tmp));

	widthOfFrame = *(unsigned int*)tmp;
	heightOfFrame = *((unsigned int*)tmp + 1);
	char tmpCompress = *(tmp + sizeof(unsigned int) * 2);

	if (tmpCompress == 'c')
	{
		compressedFile = true;
	}
}

void PlayFromFile::drawDepth(const std::vector<UINT16> &depthBuffer, const unsigned int frameCount)
{
	cvDepthMatRaw = cv::Mat(heightOfFrame, widthOfFrame, CV_16UC1, (char*)depthBuffer.data());
	cvDepthMatRaw.convertTo(cvDepthMatConversed, CV_8U, -255.0 / 8000.0, 255.0);
	cv::applyColorMap(cvDepthMatConversed, cvDepthMatConversed, cv::COLORMAP_OCEAN);
	//checkFrame();
	drawInfo(frameCount);
}

void PlayFromFile::drawInfo(const unsigned int frameCount)
{
	for (unsigned int i{ 1 }; i <= 3; i++)
	{
		cv::line(cvDepthMatConversed, cv::Point(0, (heightOfFrame / 4)*i), cv::Point(widthOfFrame, (heightOfFrame / 4)*i), 0, 1, 8, 0);
	}
	
	for (unsigned int i{ 1 }; i <= 7; i++)
	{
		cv::line(cvDepthMatConversed, cv::Point((widthOfFrame / 8)*i, 0), cv::Point((widthOfFrame / 8)*i, heightOfFrame), 0, 1, 8, 0);
	}

	std::string numberToDisplay;
	unsigned int sectorNumber{ 1 };
	for (unsigned int i{ 1 }; i <= 4; i++)
	{
		for (unsigned int j{ 1 }; j <= 8; j++)
		{
			numberToDisplay = std::to_string(sectorNumber);
			cv::putText(cvDepthMatConversed, numberToDisplay, cv::Point(((widthOfFrame / 8)*j) - 20, (heightOfFrame / 4)*i), 1, 1.0, 0, 1, 8, false);
			sectorNumber++;
		}
	}

	numberToDisplay = std::to_string(frameCount);
	cv::putText(cvDepthMatConversed, numberToDisplay, cv::Point(widthOfFrame - 60, heightOfFrame - 20), 1, 1.0, 50, 1, 8, false);
}

void PlayFromFile::checkFrame()
{
	std::vector<Sectors> sectors;
	m_checker.average(cvDepthMatRaw, sectors, heightOfFrame, widthOfFrame);
	
	for (int i{ 0 }; i < sectors.size(); i++)
	{
		cv::rectangle(cvDepthMatConversed, cv::Point(sectors[i].x1, sectors[i].y1), cv::Point(sectors[i].x2, sectors[i].y2), cv::Scalar(0, sectors[i].value, 255), 2, 0, 0);
	}
}