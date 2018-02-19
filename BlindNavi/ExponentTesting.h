#pragma once

#include <math.h>

#include "Rle.h"
#include "TestFromFile.h"

unsigned int exponent(std::vector<UINT16> sector)
{
	unsigned int sectorValue{ 0 }, relevantValue{ 0 };
	for (unsigned int i{ 0 }; i < sector.size(); i++)
	{
		if (sector[i] != 0) 
		{
			sectorValue += 1 / (exp(sector[i]/1000));
			relevantValue++;
		}
	}
	if (relevantValue == 0) { return 0; }
	else { return normalizeDanger((sectorValue / relevantValue) * 1000); }
}


void TestFromFile::exponentTesting()
{
	auto begin = std::chrono::high_resolution_clock::now();
	inputFile.clear();
	inputFile.seekg(startPosition, std::ios::beg);

	std::size_t frameSize, frameCount{ 0 }, progressCounter;
	std::vector<UINT16> compressedBuffer(widthOfFrame * heightOfFrame * 2), depthBuffer(widthOfFrame * heightOfFrame), oneSector((widthOfFrame / 8) * (heightOfFrame / 4));
	std::streampos beginInputPosition{ inputFile.tellg() }, currentInputPosition;

	unsigned int sectorValue, failedSectors{ 0 }, dangerLevel;
	
	std::cout << std::endl << "Exponent Testing" << std::endl;
	outputResults << "Exponent Testing:" << std::endl;
	
	while (inputFile.eof() == false)
	{
		inputFile.read((char*)&frameSize, sizeof(frameSize));
		inputFile.read((char*)compressedBuffer.data(), frameSize * sizeof(UINT16));
		frameCount++;

		currentInputPosition = inputFile.tellg();
		progressCounter = currentInputPosition - beginInputPosition;
		drawBar((float)progressCounter / (float)fileSize);

		if (compressedFile) { RleCompression::decompress(compressedBuffer, frameSize, depthBuffer); }
		else { memcpy(depthBuffer.data(), compressedBuffer.data(), frameSize); }

		for (int i{ 0 }; i < testSectors.size(); i++)
		{
			BufferOps::wrapSector(depthBuffer, oneSector, testSectors[i].sectorNumber, widthOfFrame, heightOfFrame);
			dangerLevel = exponent(oneSector);
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













#pragma once

#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;
//Predpokladam, ze hodnoty hlbok su medzi 0-1, teda znormalizovane podla Gaba. Gabo vie... ask him...
//ako  napoveda tento kod "double fPixel = val / 8000.0;" pri vytvarani depthMatrixForSector v app.cpp
//nenormalizujte to inak, proste /8000.0 je to co chceme, inac moje magicke konstanty stratia svoju magiu. dik
class SimonMagic
{
private:
	struct PointsStat
	{
		unsigned m1 = 0;
		unsigned m2 = 0;
		unsigned m3 = 0;
		unsigned m4 = 0;
		unsigned m5 = 0;
		unsigned m6 = 0;
		unsigned m7 = 0;
		unsigned m8 = 0;
		unsigned m9 = 0;
		unsigned m10 = 0;
	};

	PointsStat m_statOfPoints;
	//constants used in function are pure magic, based on my observations
	//for more see http://necyklopedia.wikia.com/wiki/Bulharsk%C3%A1_kon%C5%A1tanta
	double(*magicValueFunction)(double) =
		[](double depth) -> double {
		return exp(-(depth * 28 - 20));
	};

	double getStat(vector<vector<double>> & inputDepthMatrix, double(*valueOfDepthFunction)(double))
	{
		unsigned countOfMeaningfulDepths = 0;
		double depthsStat = 0.0;

		for (auto row : inputDepthMatrix)
		{
			for (auto depth : row)
			{
				if (depth == 0.0 || depth == 1.0)
				{
					continue;
				}

				countOfMeaningfulDepths++;
				_addToPointStat(depth);
				double val = valueOfDepthFunction(depth);
				depthsStat += val;
			}
		}

		double hovadskyVela = valueOfDepthFunction(0.0);
		if (countOfMeaningfulDepths == 0)
		{
			return hovadskyVela;
		}
		return depthsStat / (double)countOfMeaningfulDepths;
	}
public:

	//pozor, ten danger level je prudko exponenxialne klesajuca/rastuca funkcia, zalezi od pohladu...
	//nejak hodnoty, tak aj malo (cca 5%) hodnot okolo rozsahu 0.2 vie prebit zvysok ak je najviac 0.3
	// treshold taky, ze "uz sa nieco deje je": 0.00367 (toto je seriozne poratane....)
	double getDangerLevel(vector<vector<double>> & depthMatrixForSector)
	{
		return getStat(depthMatrixForSector, magicValueFunction) / magicValueFunction(0.0);
	}

	double getMagicFunctionTreshold()
	{
		return magicValueFunction(0.2) / magicValueFunction(0.0);
	}


	//funkcie s prefixom _ su moje testovacie
	void _printPointStats()
	{
		cout << "Point count stats" << endl;
		cout << "0.1 - " << m_statOfPoints.m1 << endl;
		cout << "0.2 - " << m_statOfPoints.m2 << endl;
		cout << "0.3 - " << m_statOfPoints.m3 << endl;
		cout << "0.4 - " << m_statOfPoints.m4 << endl;
		cout << "0.5 - " << m_statOfPoints.m5 << endl;
		cout << "0.6 - " << m_statOfPoints.m6 << endl;
		cout << "0.7 - " << m_statOfPoints.m7 << endl;
		cout << "0.8 - " << m_statOfPoints.m8 << endl;
		cout << "0.9 - " << m_statOfPoints.m9 << endl;
		cout << "1.0 - " << m_statOfPoints.m10 << endl;
	}

	//pomocna fcia, pre istotu si nechavam
	void _printValueFunctionStat()
	{
		cout << "Val function stats" << endl;
		for (double d = 0; d <= 1.0; d += 0.1)
		{
			cout << setprecision(5) << d << " - " << setprecision(20) << magicValueFunction(d) << endl;
		}
	}

	void _addToPointStat(double pt)
	{
		if (pt <= 0.1)
		{
			m_statOfPoints.m1++;
		}
		else if (pt <= 0.2)
		{
			m_statOfPoints.m2++;
		}
		else if (pt <= 0.3)
		{
			m_statOfPoints.m3++;
		}
		else if (pt <= 0.4)
		{
			m_statOfPoints.m4++;
		}
		else if (pt <= 0.5)
		{
			m_statOfPoints.m5++;
		}
		else if (pt <= 0.6)
		{
			m_statOfPoints.m6++;
		}
		else if (pt <= 0.7)
		{
			m_statOfPoints.m7++;
		}
		else if (pt <= 0.8)
		{
			m_statOfPoints.m8++;
		}
		else if (pt <= 0.9)
		{
			m_statOfPoints.m9++;
		}
		else if (pt <= 1.0)
		{
			m_statOfPoints.m10++;
		}
	}

};