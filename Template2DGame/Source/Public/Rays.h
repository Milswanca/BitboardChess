#pragma once
#include "ChessTypes.h"
#include <array>

class Rays
{
public:
	static void Init();
	static uint64_t GetRay(int Square, ERays Direction);

private:
	static uint64_t RayMap[64][8];
};