#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <wrl\client.h>
#include <iomanip>
#include <chrono>

#include "Sectors.h"
#include "Checker.h"

class TestFromFile
{
private:
	std::ifstream inputFile, inputConfig;
	std::ofstream outputResults;
	std::vector<TestSectors> testSectors;
	std::streampos startPosition;
	std::size_t fileSize;
	Checker m_checker;
	unsigned int heightOfFrame, widthOfFrame;
	bool compressedFile{ false };

public:
	TestFromFile() {}
	~TestFromFile();
	void run(std::string fileToRead, std::string configToRead);

private:
	std::size_t getFileSize();
	const std::string getTime();
	void openFile(const std::string fileName);
	void createFile();
	void getAtributes();
	void readConfig();
	void drawBar(const float progress);
	const void runStatistic(const std::size_t frameCount, const unsigned int failedSectors, const double duration);
	void runTests();
	void averageTesting();
	void maximumTesting();
	void exponentTesting();
};