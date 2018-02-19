#include "stdafx.h"
#include "KinectInit.h"
#include "Sectors.h"
#include "opencv2\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "RealTime.h"



#include <stdio.h>
#include <atlbase.h>
#include <atlcom.h>
#include <sapi.h>
#include <string>


RealTime::RealTime()
{}

RealTime::~RealTime()
{}

void RealTime::run()
{
	while (heightOfFrame == NULL && widthOfFrame == NULL)
	{
		heightOfFrame = m_kinect.getHeight();
		widthOfFrame = m_kinect.getWidth();
		
	}

	depthBuffer.resize(heightOfFrame * widthOfFrame);

	while (true)
	{
		m_kinect.getFrame(depthBuffer);
		currentTimeSpan = m_kinect.getTime();
		
		if (currentTimeSpan == lastTimeSpan)
		{
			continue;
		}
		lastTimeSpan = currentTimeSpan;

		DrawDepth();
		CheckFrame();
		ShowDepth();

		const int key = cv::waitKey(1);
		if (key == VK_ESCAPE) 
			break;
	}
}

void RealTime::DrawDepth()
{
	depthMapRaw = cv::Mat(heightOfFrame, widthOfFrame, CV_16UC1, &depthBuffer[0]);
	depthMapRaw.convertTo(depthMapConversed, CV_8U, -255.0 / 8000.0, 255.0);
	cv::applyColorMap(depthMapConversed, depthMapConversed, cv::COLORMAP_BONE);
}

void RealTime::CheckFrame()
{
	std::vector<Sectors> sectors;
	m_checker.average(depthMapRaw, sectors, heightOfFrame, widthOfFrame);
	RctlToDepth(sectors);
}

void RealTime::RctlToDepth(std::vector<Sectors> sectors)
{
	for (int i{ 0 }; i < sectors.size(); i++)
	{
		cv::rectangle(depthMapConversed, cv::Point(sectors[i].x1, sectors[i].y1), cv::Point(sectors[i].x2, sectors[i].y2), cv::Scalar(0, sectors[i].value, 255), 2, 0, 0);
	}
}

void RealTime::ShowDepth()
{
	cv::imshow("BlindNavi", depthMapConversed);
}