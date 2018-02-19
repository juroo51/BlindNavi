#pragma once

#include "TestFromFile.h"
#include "Rle.h"

void TestFromFile::maximumTesting()
{
	auto begin = std::chrono::high_resolution_clock::now();
	inputFile.clear();
	inputFile.seekg(startPosition, std::ios::beg);

	std::vector<UINT16> compressedBuffer(widthOfFrame * heightOfFrame * 2), depthBuffer(widthOfFrame * heightOfFrame), oneSector((widthOfFrame / 8) * (heightOfFrame / 4));
	std::size_t frameSize, progressCounter;
	std::streampos currentInputPosition, beginInputPosition{ inputFile.tellg() };
	UINT16 maxValue{ 0 };
	unsigned int frameCount{ 0 }, failedSectors{ 0 }, dangerLevel;


	std::cout << std::endl << "Maximum Testing" << std::endl;
	outputResults << "Maximum Testing:" << std::endl;
	
	while (inputFile.eof() != true)
	{
		inputFile.read((char*)&frameSize, sizeof(frameSize));
		inputFile.read((char*)compressedBuffer.data(), frameSize * sizeof(UINT16));
		frameCount++;

		currentInputPosition = inputFile.tellg();
		progressCounter = currentInputPosition - beginInputPosition;
		drawBar((float)progressCounter / (float)fileSize);

		if (compressedFile) { RleCompression::decompress(compressedBuffer, frameSize, depthBuffer); }
		else { memcpy(depthBuffer.data(), compressedBuffer.data(), frameSize * sizeof(UINT16)); }

		for (int i{ 0 }; i < testSectors.size(); i++)
		{
			maxValue = 0;
			BufferOps::wrapSector(depthBuffer, oneSector, testSectors[i].sectorNumber, widthOfFrame, heightOfFrame);
			
			for (int j{ 0 }; j < oneSector.size(); j++)
			{
				if (oneSector[j] > maxValue) { oneSector[i] = maxValue; }
			}

			dangerLevel = normalizeDanger(maxValue);

			if (frameCount >= testSectors[i].testFrom && frameCount <= testSectors[i].testTo)
			{
				if (dangerLevel != testSectors[i].value)
				{
					outputResults << "Fail on Frame: " << frameCount << " - sector: " << testSectors[i].sectorNumber << " - correct: " << testSectors[i].value << " - actual: " << dangerLevel << std::endl;
					failedSectors++;
				}
			}
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	double duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000;
	runStatistic(frameCount, failedSectors, duration);
}