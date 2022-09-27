#pragma warning(disable:4146)

#include "BitboardUtils.h"
#include <array>

constexpr auto BitCounterArray{
[]() constexpr
{
	std::array<unsigned char, 32768> Result{};
	for (unsigned short i = 0; i < 32768; ++i)
	{
		unsigned short v = i;
		int c = 0;

		while (v)
		{
			c++;
			v &= v - 1;
		}

		Result[i] = (unsigned char)c;
	}
	return Result;
}()
};

void BitboardUtils::Pop(uint64_t& Bitboard, int Index)
{
	(Bitboard) &= ~(1ULL << (Index));
}

void BitboardUtils::Set(uint64_t& Bitboard, int Index)
{
	Bitboard |= (1ULL << Index);
}

bool BitboardUtils::Get(uint64_t Bitboard, int Index)
{
	return ((Bitboard >> Index) & 1);
}

void BitboardUtils::Move(uint64_t& Bitboard, int From, int To)
{
	// Clear the to bit
	Bitboard &= ~(1ULL << To);

	// Set the to bit to 1 if it is on in from
	Bitboard |= ((uint64_t)Get(Bitboard, From) << To);

	// Clear the from bit
	Bitboard &= ~(1ULL << From);
}

int BitboardUtils::Count(uint64_t Bitboard)
{
	// std::array size caps out at 65535 and we need 65536 elements if we want to include zero.
	// If we don't include zero we need to instead check for 0 and continue which is probably slower than just shifting by 15s.
	unsigned short s1 = (Bitboard & 0b0000000000000000000000000000000000000000000000000111111111111111) >> 0;
	unsigned short s2 = (Bitboard & 0b0000000000000000000000000000000000111111111111111000000000000000) >> 15;
	unsigned short s3 = (Bitboard & 0b0000000000000000000111111111111111000000000000000000000000000000) >> 30;
	unsigned short s4 = (Bitboard & 0b0000111111111111111000000000000000000000000000000000000000000000) >> 45;
	unsigned short s5 = (Bitboard & 0b1111000000000000000000000000000000000000000000000000000000000000) >> 60;

	unsigned char v1 = BitCounterArray.at(s1);
	unsigned char v2 = BitCounterArray.at(s2);
	unsigned char v3 = BitCounterArray.at(s3);
	unsigned char v4 = BitCounterArray.at(s4);
	unsigned char v5 = BitCounterArray.at(s5);

	return v1 + v2 + v3 + v4 + v5;
}

int BitboardUtils::GetLSBitIndex(uint64_t Bitboard)
{
	unsigned long Index;
	_BitScanReverse64(&Index, Bitboard);
	return Index;
}

int BitboardUtils::GetMSBitIndex(uint64_t Bitboard)
{
	unsigned long Index;
	_BitScanForward64(&Index, Bitboard);
	return Index;
}

void BitboardUtils::ForEach(uint64_t Bitboard, std::function<void(int)> Func)
{
	uint64_t Copy = Bitboard;
	int c = Count(Bitboard);
	for (int i = 0; i < c; ++i)
	{
		int Idx = GetLSBitIndex(Copy);
		Func(Idx);
		Pop(Copy, Idx);
	}
}