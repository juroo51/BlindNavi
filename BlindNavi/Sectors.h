#pragma once
class Sectors
{
public:
	int x1, y1, x2, y2;
	double value;
};

class TestSectors
{
public:
	unsigned int testFrom, testTo, value, sectorNumber;
};

class BufferOps
{
public:
	static void wrapSector(const std::vector<UINT16>& inputVector, std::vector<UINT16>& outputVector, short position, const int widthOfFrame, const int heightOfFrame)
	{
		position--;
		int row{ position / 8 };
		int column{ position % 8 };
		int startingPoint{ (row * widthOfFrame * (heightOfFrame / 4)) + (column * widthOfFrame / 8) };

		for (int i{ 0 }; i < outputVector.size();)
		{
			for (int j{ 0 }; j < (widthOfFrame / 8); j++)
			{
				outputVector[i] = inputVector[startingPoint + j];
				i++;
			}
			startingPoint += widthOfFrame;
		}
	}
};

//class VladoDepthBufferMatrix
//{
//	UINT16* m_buffer;
//	int m_width;
//	int m_height;
//
//public:
//	VladoDepthBufferMatrix(UINT16* depthBuffer, int width, int height) : m_buffer(depthBuffer), m_width(width), m_height(height)
//	{}
//
//	~VladoDepthBufferMatrix()
//	{
//		delete m_buffer;
//	}
//
//	UINT16 get(int x, int y)
//	{
//		std::vector<int> vec;
//		vec.resize(200);
//		x = vec[53];
//
//		return *(m_buffer + (y % m_height)*m_width + x);
//	}
//};
