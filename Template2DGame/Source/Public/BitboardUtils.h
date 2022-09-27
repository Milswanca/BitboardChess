#pragma once
#include <glm/glm.hpp>
#include <functional>

class BitboardUtils
{
public:
	static void Pop(uint64_t& Bitboard, int Index);
	static void Set(uint64_t& Bitboard, int Index);
	static bool Get(uint64_t Bitboard, int Index);
	static void Move(uint64_t& Bitboard, int From, int To);
	static int Count(uint64_t Bitboard);
	static int GetLSBitIndex(uint64_t Bitboard);
	static int GetMSBitIndex(uint64_t Bitboard);
	static void ForEach(uint64_t Bitboard, std::function<void(int)> Func);
};

