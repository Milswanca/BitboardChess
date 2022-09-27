#pragma once
#include "ChessTypes.h"
#include <array>

class Rays
{
private:
	static constexpr std::array<std::array<uint64_t, 8>, 64> RayMap
	{
		[]() constexpr
		{
			std::array<std::array<uint64_t, 8>, 64> result{};
			int DirX[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
			int DirY[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

			for (int i = 0; i < 64; ++i)
			{
				for (int j = 0; j < 8; ++j)
				{
					int tx = (i % 8);
					int ty = (i / 8);

					tx += DirX[j];
					ty += DirY[j];

					while((tx >= 0 && tx <= 7) && (ty >= 0 && ty <= 7))
					{
						result[i][j] |= (1ULL << (tx + (ty * 8)));

						tx += DirX[j];
						ty += DirY[j];
					}
				}
			}

			return result;
		}()
	};

public:
	static uint64_t GetRay(int Square, ERays Direction);
};