#pragma once
#include <vector>
#include <wrl\client.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>

#include "Sectors.h"

class Checker
{
public:
	void average(const cv::Mat& depthBuffer, std::vector<Sectors>& sectors, int heightOfFrame, int widthOfFrame)
	{

		int count{ 0 }, chck{ 0 };
		long int sum{ 0 };
		double avg;

		//check whole MAT
		for (int s = 0; s < 8; s++)
			for (int r = 0; r < 4; r++)
			{
				sum = 0;
				count = 0;

				//check one sector
				for (int a = 0 + ((heightOfFrame / 4)*r); a < ((heightOfFrame/ 4)*(r + 1)); a++)
				{
					for (int b = (widthOfFrame/ 8)*s; b < (widthOfFrame / 8)*(s + 1); b++)
					{
						//0 values cancellation
						if (depthBuffer.at <unsigned short>(a, b) != 0)
						{
							sum += depthBuffer.at <unsigned short>(a, b);
							count++;
						}
					}
				}

				//noise cancellation
				if (count >= 200)
				{
					avg = sum / count;

					//average value of sector in interval 500-3500
					if (avg <= 3500.0 && avg >= 500.0)
					{
						avg = (((avg - 500) / 3000) * 255);
						sectors.push_back({ (widthOfFrame / 8)*s, (heightOfFrame / 4)*r, (widthOfFrame / 8)*(s + 1), (heightOfFrame / 4)*(r + 1), avg });
					}
				}
				else
					chck++;
			}
		/////new
		////check whole vector
		//for (unsigned int row{ 0 }; row < 4; row++)
		//{
		//	for (unsigned int column{ 0 }; column < 8; column++)
		//	{
		//		//check one sector
		//		for (unsigned int i{ widthOfFrame / 8 + widthOfFrame * column }; i < (widthOfFrame / 8) * 2 + widthOfFrame * column; i++)
		//		{
		//			for (unsigned  int j{ heightOfFrame / 4 + heightOfFrame * row }; j < (heightOfFrame / 4) * 2 + heightOfFrame * row; j++)
		//			{
		//				//unsigned int from, to;

		//			}
		//		}
		//	}
		//}

	}
};