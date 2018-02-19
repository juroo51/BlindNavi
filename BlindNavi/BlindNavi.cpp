#define _ATL_APARTMENT_THREADED

#include "stdafx.h"
#include "RealTime.h"
#include "KinectInit.h"
#include "StoreToFile.h"
#include "PlayFromFile.h"
#include "TestFromFile.h"

int main()
{
	bool realTimeMode		{ true	};
	bool storeToFileMode	{ false	};
	bool compressMode		{ false	};
	bool playFromFileMode	{ false	};
	bool testFromFileMode	{ false	};
	std::string fileToRead	{ "06.02.2018_12-12-45.jdf" };
	std::string configToRead{ "config.txt" };

	if (realTimeMode)
	{
		RealTime realtimemode;
		realtimemode.run();
		return 0;
	}

	if (storeToFileMode)
	{
		StoreToFile storetofilemode;
		storetofilemode.run(compressMode);
		return 0;
	}

	if (playFromFileMode)
	{
		PlayFromFile playfromfilemode;
		playfromfilemode.run(fileToRead);
		return 0;
	}

	if (testFromFileMode)
	{
		TestFromFile testfromfilemode;
		testfromfilemode.run(fileToRead, configToRead);
		system("pause");
		return 0;
	}

	return 0;
}