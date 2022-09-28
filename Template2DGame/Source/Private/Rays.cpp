#include "Rays.h"

uint64_t Rays::RayMap[64][8];

void Rays::Init()
{
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

			while ((tx >= 0 && tx <= 7) && (ty >= 0 && ty <= 7))
			{
				RayMap[i][j] |= (1ULL << (tx + (ty * 8)));

				tx += DirX[j];
				ty += DirY[j];
			}
		}
	}
}

uint64_t Rays::GetRay(int Square, ERays Direction)
{
	return RayMap[Square][static_cast<int>(Direction)];
}