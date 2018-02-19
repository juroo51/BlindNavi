#pragma once

#include <vector>

#include "opencv2\opencv.hpp"
#include "KinectInit.h"
#include "Checker.h"

class RealTime
{
private:
	cv::Mat depthMapRaw, depthMapConversed;;
	std::vector<UINT16> depthBuffer;
	Kinect m_kinect;
	Checker m_checker;
	unsigned int heightOfFrame, widthOfFrame;
	TIMESPAN currentTimeSpan, lastTimeSpan{ 0 };

public:
	RealTime();
	~RealTime();
	void run();

private:
	void DrawDepth();
	void CheckFrame();
	void RctlToDepth(std::vector <Sectors>);
	void ShowDepth();
};