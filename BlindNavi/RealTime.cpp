#include "stdafx.h"
#include "RealTime.h"
#include "KinectInit.h"
#include "opencv2\highgui.hpp"
#include "opencv2\opencv.hpp"


RealTime::RealTime()
{}

RealTime::~RealTime()
{
	cvDestroyAllWindows();
}

void RealTime::run()
{
	while (true)
	{
		UpdateDepth();
		DrawDepth();
		CheckFrame();
		//RctlToDepth();
		//ShowDepth();
	}
}

void RealTime::UpdateDepth()
{
	m_kinect.getFrame(depthBuffer);
	HeightOfFrame = m_kinect.getHeight();
	WidthOfFrame = m_kinect.getWidth();
}

void RealTime::DrawDepth()
{
	depthMapRaw = cv::Mat(HeightOfFrame, WidthOfFrame, CV_16UC1, &depthBuffer[0]);
	depthMapRaw.convertTo(depthMapConversed, CV_8U, -255.0 / 8000.0, 255.0);
	cv::applyColorMap(depthMapConversed, depthMapConversed, cv::COLORMAP_BONE);
}

void RealTime::CheckFrame()
{

//	m_checker.run(depthMapRaw);
}