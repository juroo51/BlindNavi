#pragma once
#include <vector>

#include"opencv2\opencv.hpp"

class Checker
{
public:

	void average(cv::Mat depthMat)
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
				for (int a = 0 + ((424 / 4)*r); a < ((424 / 4)*(r + 1)); a++)
				{
					for (int b = (512 / 8)*s; b < (512 / 8)*(s + 1); b++)
					{
						//0 values cancellation
						if (depthMat.at <unsigned short>(a, b) != 0)
						{
							sum += depthMat.at <unsigned short>(a, b);
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
						//do magic
					}
				}
				else
					chck++;
			}
	}
};