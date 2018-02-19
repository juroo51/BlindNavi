#pragma once
#include <fstream>
#include <vector>
#include <string>

#include "KinectInit.h"

class StoreToFile
{
private:
	Kinect m_kinect;
	TIMESPAN currentTimeSpan{ 0 }, lastTimeSpan{ 0 };
	size_t frameSize;
	std::ofstream output;
	unsigned int width, height, frameCount{ 1 };
	bool compressMode;

public:
	StoreToFile();
	~StoreToFile();
	void run(bool);

private:
	const std::string getTime();
	void openFile();
	void saveFrame(const std::vector<UINT16>& compressedBuffer, size_t sizeToWrite);
	void saveAtributes();
};