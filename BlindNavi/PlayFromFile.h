#pragma once
#include "Checker.h"

#include <vector>
#include <wrl\client.h>
#include <fstream>
#include <string>
#include <opencv2\opencv.hpp>

class PlayFromFile
{
private:
	cv::Mat cvDepthMatRaw, cvDepthMatConversed;
	std::ifstream input;
	std::size_t frameSize;
	Checker m_checker;
	unsigned int widthOfFrame, heightOfFrame, frameRate{ 24 };
	bool compressedFile{ false };

public:
	PlayFromFile() {}
	~PlayFromFile();
	void run(std::string file);

private:
	void openFile(std::string file);
	void getAtributes();
	void drawDepth(const std::vector<UINT16>& depthBuffer, const unsigned int frameCount);
	void drawInfo(const unsigned int frameCount);
	void checkFrame();
	void pause();
	void hold();
};