#include "stdafx.h"
#include "RealTime.h"
#include "KinectInit.h"
#include <iostream>

#include <vector>

int main()
{
	void tesst();

	bool RealTimeMode{ false };
	bool StoreToFileMode{ false };
	bool TestFromFileMode{ false };

	if (RealTimeMode)
	{
		RealTime realtimemode;
		realtimemode.run();
	}

	if (StoreToFileMode)
	{

	}

	if (TestFromFileMode)
	{

	}

    return 0;
}

