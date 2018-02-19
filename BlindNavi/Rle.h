#pragma once
#include <vector>

class RleCompression
{
public:

	static std::size_t compress(const std::vector<UINT16>& depthBuffer, std::vector<UINT16>& compressedBuffer)
	{
		unsigned int count{ 1 };

		auto currentIt = compressedBuffer.begin();

		for (int i{ 0 }; i < depthBuffer.size(); i++)
		{
			if (i == (depthBuffer.size() - 1) || depthBuffer[i] != depthBuffer[i + 1])
			{
				*currentIt = count; currentIt++;
				*currentIt = depthBuffer[i]; currentIt++;
				count = 1;
			}
			else
			{
				count++;
			}
		}

		return currentIt - compressedBuffer.begin();
	}

	static void decompress(const std::vector<UINT16>& compressedBuffer, std::size_t compressedBufferSize, std::vector<UINT16>& depthBuffer)
	{
		auto position = depthBuffer.begin();
		unsigned int value;

		for (unsigned int i{ 0 }; i < compressedBufferSize; i += 2)
		{
			value = compressedBuffer[i + 1];
			std::fill(position, position + compressedBuffer[i], value);
			position += compressedBuffer[i];
		}
	}
};