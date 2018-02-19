#include "stdafx.h"
#include "StoreToFile.h"
#include "Rle.h"

#include <chrono>
#include <iostream>

void StoreToFile::run(bool compress)
{
	compressMode = compress;
	openFile();
	
	while (width == NULL && height == NULL)
	{
		width = m_kinect.getWidth();
		height = m_kinect.getHeight();
	}
	
	saveAtributes();
	
	std::vector<UINT16> depthBuffer(width * height);
	std::vector<UINT16> compressedBuffer(width * height * 2);

	bool escPressed{ false };

	while (!escPressed)
	{
		m_kinect.getFrame(depthBuffer);
		
		currentTimeSpan = m_kinect.getTime();
		if (currentTimeSpan == lastTimeSpan)
		{
			continue;
		}
		lastTimeSpan = currentTimeSpan;

		if (compressMode)
		{
			frameSize = RleCompression::compress(depthBuffer, compressedBuffer);
			saveFrame(compressedBuffer, frameSize);
		}
		else
		{
			frameSize = width * height;
			saveFrame(depthBuffer, frameSize);
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			escPressed = true;
		}
	}
}

void StoreToFile::saveAtributes()
{
	char tmpCompress;
	if (compressMode)
	{
		tmpCompress = 'c';
	}
	else
	{
		tmpCompress = 'd';
	}

	output.write((char*)&width, sizeof(width));
	output.write((char*)&height, sizeof(height));
	output.write((char*)&tmpCompress, sizeof(tmpCompress));
}

void StoreToFile::saveFrame(const std::vector<UINT16>& outputBuffer, size_t sizeToWrite)
{	
	std::cout << frameCount << std::endl;

	output.write((const char*)&frameSize, sizeof(frameSize));
	output.write((const char*)outputBuffer.data(), frameSize * sizeof(UINT16));
	
	frameCount++;
}

const std::string StoreToFile::getTime()
{
	time_t currentTime = time(0);
	struct tm timeStruct;
	char timeBuffer[80];
	timeStruct = *localtime(&currentTime);
	strftime(timeBuffer, sizeof(timeBuffer), "%d.%m.%Y_%H-%M-%S", &timeStruct);
	return timeBuffer;
}

void StoreToFile::openFile()
{
	std::string path{ "C:/Users/jlipovsky/Desktop/TestVids/" };
	path += getTime();
	if (compressMode) { path += "com"; }
	path += ".jdf";
	output.open(path.c_str(), std::ios::binary);
	if (!output.is_open()) { std::cout << "Unable to open file." << std::endl; system("pause"); };
}

StoreToFile::StoreToFile()
{}

StoreToFile::~StoreToFile()
{}