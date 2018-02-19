#include "stdafx.h"
#include "TestFromFile.h"
#include "Rle.h"
#include "AverageTesting.h"
#include "MaximumTesting.h"
#include "ExponentTesting.h"

void TestFromFile::run(std::string fileToRead, std::string configToRead)
{
	openFile(fileToRead);
	openFile(configToRead);

	if (!inputFile.is_open() && !inputConfig.is_open())
	{
		std::cout << "Unable to open files" << std::endl;
		system("pause");
		return;
	}	
	fileSize = getFileSize();
	std::cout << "Size of InputFile: " << fileSize/ 1024 << " KB" << std::endl;
	
	readConfig();
	getAtributes();
	createFile();
	startPosition = inputFile.tellg();
	runTests();
}

void TestFromFile::runTests()
{
	averageTesting();
	maximumTesting();
	exponentTesting();
}

void TestFromFile::openFile(const std::string fileName)
{
	std::string filePath{ "C:/Users/jlipovsky/Desktop/TestVids/" };
	filePath += fileName;
	
	if (fileName.find("jdf") != std::string::npos)
	{
		inputFile.open(filePath.c_str(), std::fstream::binary);
	}
	else
	{
		inputConfig.open(filePath.c_str());
	}
}

std::size_t TestFromFile::getFileSize()
{
	std::streampos begin, end;
	inputFile.seekg(0, std::ios::beg);
	begin = inputFile.tellg();
	inputFile.seekg(0, std::ios::end);
	end = inputFile.tellg();
	inputFile.seekg(0, std::ios::beg);
	return end - begin;
}

void TestFromFile::getAtributes()
{
	char tmpBuffer [sizeof(unsigned int) * 2 + sizeof(char)];
	inputFile.read((char*)&tmpBuffer, sizeof(tmpBuffer));

	widthOfFrame = *(unsigned int*)tmpBuffer;
	heightOfFrame = *((unsigned int*)tmpBuffer + 1);
	char tmpCompress = *(tmpBuffer + sizeof(unsigned int) * 2);

	if (tmpCompress == 'c') { compressedFile = true; }
}

void TestFromFile::readConfig()
{
	unsigned int from, to, value, number;

	while(!inputConfig.eof())
	{
		inputConfig >> from >> to >> value >> number;
		testSectors.push_back({ from, to, value, number });
	}
}

const void TestFromFile::runStatistic(const std::size_t frameCount, const unsigned int failedSectors, const double duration)
{
	if (failedSectors == 0)
	{
		outputResults << "No test Fails" << std::endl;
	}

	double accuracy = (double)failedSectors / ((double)frameCount * 32);

	outputResults << "-----------------------------------------------------" << std::endl;
	outputResults << "Tested Frames:		" << frameCount << std::endl << "Failed Sectors:		" << failedSectors << std::endl << "Accuracy:		" << accuracy << "%" << std::endl;
	outputResults << "Duration:		" << duration << " sec" << std::endl;
	outputResults << "=====================================================" << std::endl << std::endl;
	std::cout << std::endl << "Failed Sectors: " << std::setprecision(4) << accuracy << "%" << std::endl;
	std::cout << "Duration: " << duration << " sec" << std::endl << std::endl;
}

void TestFromFile::drawBar(const float progress)
{
	if (progress > 1) { return; }
	unsigned int barWidth = 70;

	std::cout << "[";
	unsigned int progressBar = barWidth * progress;
	for (unsigned int i = 0; i < barWidth; ++i)
	{
		if (i < progressBar) { std::cout << "="; }
		else
		{
			if (i == progressBar) { std::cout << ">"; }
			else { std::cout << " "; }
		}
	}
	std::cout << "] " << int(progress * 100.0) << " %\r";
}

const std::string TestFromFile::getTime()
{
	time_t currentTime = time(0);
	struct tm timeStruct;
	char timeBuffer[80];
	timeStruct = *localtime(&currentTime);
	strftime(timeBuffer, sizeof(timeBuffer), "%d.%m.%Y_%H-%M-%S", &timeStruct);
	return timeBuffer;
}

void TestFromFile::createFile()
{
	std::string path{ "C:/Users/jlipovsky/Desktop/TestVids/TestResults/" };
	path += getTime();
	path += ".txt";
	outputResults.open(path.c_str());
	if (!outputResults.is_open()) { std::cout << "Unable to open file." << std::endl; system("pause"); };
}

TestFromFile::~TestFromFile()
{
	inputConfig.close();
	inputFile.close();
	outputResults.close();
}


/*
tests
histogram

nacitavat vacsie pole z disku
urobit wrapper na buffer
*/