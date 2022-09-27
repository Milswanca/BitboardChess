#include "Rays.h"

uint64_t Rays::GetRay(int Square, ERays Direction)
{
	return RayMap[Square][static_cast<int>(Direction)];
}
