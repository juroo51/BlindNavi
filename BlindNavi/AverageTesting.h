#pragma once

#include "TestFromFile.h"
#include "Rle.h"
#include "Normalize.h"

void TestFromFile::averageTesting()
{
	auto begin = std::chrono::high_resolution_clock::now();
	inputFile.clear();
	inputFile.seekg(startPosition, std::ios::beg);

	std::size_t frameSize, frameCount{ 0 }, progressCounter;
	std::vector<UINT16> compressedBuffer(widthOfFrame * heightOfFrame * 2), depthBuffer(widthOfFrame * heightOfFrame);
	std::streampos beginInputPosition{ inputFile.tellg() }, currentInputPosition;

	unsigned int sectorValue, failedSectors{ 0 }, dangerLevel;
	unsigned short sectorCount;
	double sectorAverage;

	std::cout << std::endl << "Average Testing" << std::endl;
	outputResults << "Average Testing:" << std::endl;
	while (inputFile.eof() == false)
	{
		inputFile.read((char*)&frameSize, sizeof(frameSize));
		inputFile.read((char*)compressedBuffer.data(), frameSize * sizeof(UINT16));
		frameCount++;

		currentInputPosition = inputFile.tellg();
		progressCounter = currentInputPosition - beginInputPosition;

		drawBar((float)progressCounter / (float)fileSize);

		if (compressedFile) { RleCompression::decompress(compressedBuffer, frameSize, depthBuffer); }
		else { depthBuffer = compressedBuffer; }

		std::vector<UINT16> oneSector((widthOfFrame / 8) * (heightOfFrame / 4));

		for (unsigned int i{ 0 }; i < testSectors.size(); i++)
		{
			sectorValue = 0;
			sectorCount = 0;
			BufferOps::wrapSector(depthBuffer, oneSector, testSectors[i].sectorNumber, widthOfFrame, heightOfFrame);

			for (int i{ 0 }; i < oneSector.size(); i++)
			{
				sectorValue += oneSector[i];
				sectorCount++;
			}

			sectorAverage = sectorValue / sectorCount;	
			dangerLevel = normalizeDanger(sectorAverage);
			if (frameCount >= testSectors[i].testFrom && frameCount <= testSectors[i].testTo)
			{
				if (dangerLevel < testSectors[i].value)
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
