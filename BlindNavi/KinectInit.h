#pragma once
#include <vector>

#include "wrl\client.h"
#include "Kinect.h"

using namespace Microsoft::WRL;

class Kinect
{

private:
	ComPtr<IKinectSensor> kinect;
	ComPtr<IDepthFrameReader> depthFrameReader;
	int widthOfDepth, heightOfDepth;
	TIMESPAN relativeTime;

public:
	Kinect()
	{
		GetDefaultKinectSensor(&kinect);
		kinect->Open();

		ComPtr<IDepthFrameSource> depthFrameSource;
		kinect->get_DepthFrameSource(&depthFrameSource);
		depthFrameSource->OpenReader(&depthFrameReader);

		ComPtr<IFrameDescription> depthDescription;
		depthFrameSource->get_FrameDescription(&depthDescription);
		depthDescription->get_Width(&widthOfDepth);
		depthDescription->get_Height(&heightOfDepth);
		Sleep(2000);
	}

	~Kinect()
	{
		if (kinect != nullptr)
		{
			kinect->Close();
		}
	}

	int getWidth() const
	{
		return widthOfDepth;
	}

	int getHeight() const
	{
		return heightOfDepth;
	}

	TIMESPAN getTime() 
	{
		return relativeTime;
	}

	void getFrame(std::vector<UINT16>& depthFrame)
	{
		ComPtr<IDepthFrame> tmpFrame;
		depthFrameReader->AcquireLatestFrame(&tmpFrame);

		if (tmpFrame == NULL)
		{
			return;
		}

		tmpFrame->CopyFrameDataToArray(static_cast<UINT>(depthFrame.size()), &depthFrame[0]);
		tmpFrame->get_RelativeTime(&relativeTime);
	}
};