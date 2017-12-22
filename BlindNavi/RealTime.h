#pragma once
#include <vector>

#include "opencv2\opencv.hpp"
#include "KinectInit.h"
#include "Checker.h"

class RealTime
{
private:
	cv::Mat depthMapRaw;
	cv::Mat depthMapConversed;

	std::vector<UINT16> depthBuffer;

	Kinect m_kinect;
//	Checker m_checker;

	unsigned int HeightOfFrame;
	unsigned int WidthOfFrame;

public:
	RealTime();
	~RealTime();
	void run();

private:
	void UpdateDepth();
	void DrawDepth();
	void CheckFrame();
	//void RctlToDepth();
	//void ShowDepth();
};