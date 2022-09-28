#include "MoveGen.h"
#include "Rays.h"
#include "ChessTypes.h"
#include "BitboardUtils.h"

#include <iostream>
#include <intrin.h>

uint64_t MoveGen::VisionMask_Knight[64];
uint64_t MoveGen::VisionMask_Rook[64][4096];
uint64_t MoveGen::VisionMask_Bishop[64][1024];
uint64_t MoveGen::VisionMask_King[64];

uint64_t MoveGen::BlockersMask_Rook[64];
uint64_t MoveGen::BlockersMask_Bishop[64];

const uint64_t MoveGen::RookMagics[64] = {
	0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
	0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
	0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
	0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
	0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
	0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
	0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
	0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
	0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
	0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
	0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
	0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
	0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const int MoveGen::RookIndexBits[64] = {
	12, 11, 11, 11, 11, 11, 11, 12,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	11, 10, 10, 10, 10, 10, 10, 11,
	12, 11, 11, 11, 11, 11, 11, 12
};

const glm::uint64_t MoveGen::BishopMagics[64] = {
	0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
	0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
	0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
	0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
	0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
	0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
	0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
	0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
	0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
	0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
	0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
	0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
	0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

const int MoveGen::BishopIndexBits[64] = {
	6, 5, 5, 5, 5, 5, 5, 6,
	5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 9, 9, 7, 5, 5,
	5, 5, 7, 7, 7, 7, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 5, 5, 5, 5, 5, 5, 6
};

void MoveGen::Init()
{
	InitVision_Knight();
	InitVision_Rook();
	InitVision_Bishop();
	InitVision_King();
}

void MoveGen::GenerateMoves(UChessModel::FBoardState* State)
{
	for (int i = 0; i < 64; ++i)
	{
		State->BMVision[i] = 0;
	}

	ComputeVision_Pawn(State);
	ComputeVision_Rook(State);
	ComputeVision_Knight(State);
	ComputeVision_Bishop(State);
	ComputeVision_Queen(State);
	ComputeVision_King(State);

	State->NumMoves = 0;
	uint64_t Occupancy = State->BMWhite | State->BMBlack;
	BitboardUtils::ForEach(Occupancy, [&](int Source)
		{
			uint64_t Friendly = State->GetOwner(Source) == 1 ? State->BMWhite : State->BMBlack;
			uint64_t Attacks = State->BMVision[Source] & ~(Friendly);

			BitboardUtils::ForEach(Attacks, [&](int Target) {
				int Move = EncodeMove(Source, Target, State->GetPiece(Source), 0);
				State->Moves[State->NumMoves++] = Move;
				});
		});
}

int MoveGen::EncodeMove(int source, int target, int piece, int promotion)
{
	return (source) | (target << 6) | (piece << 12) | (promotion << 16);
}

void MoveGen::DecodeMove(int Move, int& Source, int& Target, int& Piece, int& Promotion)
{
	Source = Move & 0x3f;
	Target = (Move & 0xfc0) >> 6;
	Piece = (Move & 0xf000) >> 12;
	Promotion = (Move & 0xf0000) >> 16;
}

uint64_t GetBlockersFromIndex(int Index, uint64_t Mask)
{
	uint64_t Blockers = 0ULL;
	int Bits = BitboardUtils::Count(Mask);
	for (int i = 0; i < Bits; i++) {
		int BitPos = BitboardUtils::GetLSBitIndex(Mask);
		if (Index & (1ULL << i))
		{
			Blockers |= (1ULL << BitPos);
		}
		BitboardUtils::Pop(Mask, BitPos);
	}
	return Blockers;
}

void MoveGen::InitVision_Knight()
{
	int x[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
	int y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	for (int i = 0; i < 64; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			int tx = (i % 8) + x[j];
			int ty = (i / 8) + y[j];

			if ((tx >= 0 && tx <= 7) && (ty >= 0 && ty <= 7))
			{
				VisionMask_Knight[i] |= 1ULL << (tx + (ty * 8));
			}
		}
	}
}

void MoveGen::InitVision_Rook()
{
	for (int Square = 0; Square < 64; Square++)
	{
		BlockersMask_Rook[Square] =
			(Rays::GetRay(Square, ERays::North) & ~Rank8) |
			(Rays::GetRay(Square, ERays::South) & ~Rank1) |
			(Rays::GetRay(Square, ERays::East) & ~FileH) |
			(Rays::GetRay(Square, ERays::West) & ~FileA);
	}

	// For all squares
	for (int Square = 0; Square < 64; Square++)
	{
		// For all possible blockers for this square
		for (int BlockerIndex = 0; BlockerIndex < (1 << RookIndexBits[Square]); BlockerIndex++)
		{
			uint64_t Blockers = GetBlockersFromIndex(BlockerIndex, BlockersMask_Rook[Square]);

			uint64_t Attacks = 0;
			ERays Directions[4] = { ERays::North, ERays::South, ERays::East, ERays::West };

			for (int i = 0; i < 4; ++i)
			{
				ERays Direction = Directions[i];

				// Add all direction squares
				Attacks |= Rays::GetRay(Square, Direction);

				// Check if there are any blockers
				if (Rays::GetRay(Square, Direction) & Blockers)
				{
					// If there are find the first one
					unsigned long FirstBlocker;

					if (Direction == ERays::North || Direction == ERays::East)
					{
						FirstBlocker = BitboardUtils::GetMSBitIndex(Rays::GetRay(Square, Direction) & Blockers);
					}
					else
					{
						FirstBlocker = BitboardUtils::GetLSBitIndex(Rays::GetRay(Square, Direction) & Blockers);
					}

					// Remove Squares behind the first blocker
					Attacks &= ~(Rays::GetRay(FirstBlocker, Direction));
				}
			}

			VisionMask_Rook[Square][(Blockers * RookMagics[Square]) >> (64 - RookIndexBits[Square])] = Attacks;
		}
	}
}

void MoveGen::InitVision_Bishop()
{
	for (int Square = 0; Square < 64; Square++)
	{
		uint64_t BorderSquares = Rank1 | Rank8 | FileA | FileH;
		BlockersMask_Bishop[Square] =
			((Rays::GetRay(Square, ERays::NorthEast)) |
			(Rays::GetRay(Square, ERays::SouthEast)) |
			(Rays::GetRay(Square, ERays::SouthWest)) |
			(Rays::GetRay(Square, ERays::NorthWest))) & ~(BorderSquares);
	}

	// For all squares
	for (int Square = 0; Square < 64; Square++)
	{
		// For all possible blockers for this square
		for (int BlockerIndex = 0; BlockerIndex < (1 << BishopIndexBits[Square]); BlockerIndex++)
		{
			uint64_t Blockers = GetBlockersFromIndex(BlockerIndex, BlockersMask_Bishop[Square]);

			uint64_t Attacks = 0;
			ERays Directions[4] = { ERays::NorthEast, ERays::SouthEast, ERays::SouthWest, ERays::NorthWest };

			for (int i = 0; i < 4; ++i)
			{
				ERays Direction = Directions[i];

				// Add all direction squares
				Attacks |= Rays::GetRay(Square, Direction);

				// Check if there are any blockers
				if (Rays::GetRay(Square, Direction) & Blockers)
				{
					// If there are find the first one
					unsigned long FirstBlocker;

					if (Direction == ERays::NorthEast || Direction == ERays::NorthWest)
					{
						FirstBlocker = BitboardUtils::GetMSBitIndex(Rays::GetRay(Square, Direction) & Blockers);
					}
					else
					{
						FirstBlocker = BitboardUtils::GetLSBitIndex(Rays::GetRay(Square, Direction) & Blockers);
					}

					// Remove Squares behind the first blocker
					Attacks &= ~(Rays::GetRay(FirstBlocker, Direction));
				}
			}

			VisionMask_Bishop[Square][(Blockers * BishopMagics[Square]) >> (64 - BishopIndexBits[Square])] = Attacks;
		}
	}
}

void MoveGen::InitVision_King()
{
	for (int i = 0; i < 64; ++i)
	{
		uint64_t Square = (1ULL << i);

		uint64_t Attacks = ((Square << 7) | (Square >> 9) | (Square >> 1)) & ~(FileH);
		Attacks |= ((Square << 9) | (Square >> 7) | (Square << 1)) & ~(FileA);
		Attacks |= (Square << 8) | (Square >> 8);

		VisionMask_King[i] = Attacks;
	}
}

void MoveGen::ComputeVision_Pawn(UChessModel::FBoardState* State)
{
	uint64_t WhitePawns = State->BMPieces[0] & State->BMWhite;
	uint64_t WhitePawnsRank2 = WhitePawns & Rank2;
	uint64_t WhitePawnsNotRank2 = WhitePawns & ~(Rank2);
	uint64_t WhitePawnsNotAFile = WhitePawns & ~(FileA);
	uint64_t WhitePawnsNotHFile = WhitePawns & ~(FileH);

	// Forward Moves - White
	BitboardUtils::ForEach(WhitePawnsNotRank2, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (!BitboardUtils::Get(State->BMBlack | State->BMWhite, idx + 8))
		{
			BitboardUtils::Set(PieceVision, idx + 8);
		}
		State->BMVision[idx] |= PieceVision;
		});

	// Double Moves - White
	BitboardUtils::ForEach(WhitePawnsRank2, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (!BitboardUtils::Get(State->BMBlack | State->BMWhite, idx + 8))
		{
			BitboardUtils::Set(PieceVision, idx + 8);
			if (!BitboardUtils::Get(State->BMBlack | State->BMWhite, idx + 16))
			{
				BitboardUtils::Set(PieceVision, idx + 16);
			}
		}
		State->BMVision[idx] |= PieceVision;
		});

	// Attack Left Moves - White
	BitboardUtils::ForEach(WhitePawnsNotAFile, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (BitboardUtils::Get(State->BMBlack, idx + 7))
		{
			BitboardUtils::Set(PieceVision, idx + 7);
		}
		State->BMVision[idx] |= PieceVision;
		});

	// Attack Right Moves - White
	BitboardUtils::ForEach(WhitePawnsNotHFile, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (BitboardUtils::Get(State->BMBlack, idx + 9))
		{
			BitboardUtils::Set(PieceVision, idx + 9);
		}
		State->BMVision[idx] |= PieceVision;
		});

	uint64_t BlackPawns = State->BMPieces[0] & State->BMBlack;
	uint64_t BlackPawnsRank7 = BlackPawns & Rank7;
	uint64_t BlackPawnsNotRank7 = BlackPawns & ~(Rank7);
	uint64_t BlackPawnsNotAFile = BlackPawns & ~(FileA);
	uint64_t BlackPawnsNotHFile = BlackPawns & ~(FileH);

	// Forward Moves - Black
	BitboardUtils::ForEach(BlackPawnsNotRank7, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (!BitboardUtils::Get(State->BMBlack | State->BMWhite, idx - 8))
		{
			BitboardUtils::Set(PieceVision, idx - 8);
		}
		State->BMVision[idx] |= PieceVision;
		});

	// Double Moves - Black
	BitboardUtils::ForEach(BlackPawnsRank7, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (!BitboardUtils::Get(State->BMBlack | State->BMWhite, idx - 8))
		{
			BitboardUtils::Set(PieceVision, idx - 8);
			if (!BitboardUtils::Get(State->BMBlack | State->BMWhite, idx - 16))
			{
				BitboardUtils::Set(PieceVision, idx - 16);
			}
		}
		State->BMVision[idx] |= PieceVision;
		});

	// Attack Left Moves - Black
	BitboardUtils::ForEach(BlackPawnsNotAFile, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (BitboardUtils::Get(State->BMWhite, idx - 9))
		{
			BitboardUtils::Set(PieceVision, idx - 9);
		}
		State->BMVision[idx] |= PieceVision;
		});

	// Attack Right Moves - Black
	BitboardUtils::ForEach(BlackPawnsNotHFile, [&](int idx) {
		uint64_t PieceVision = 0u;
		if (BitboardUtils::Get(State->BMWhite, idx - 7))
		{
			BitboardUtils::Set(PieceVision, idx - 7);
		}
		State->BMVision[idx] |= PieceVision;
		});
}

void MoveGen::ComputeVision_Rook(UChessModel::FBoardState* State)
{
	BitboardUtils::ForEach(State->BMPieces[1], [&](int Index) {
		uint64_t Blockers = (State->BMBlack | State->BMWhite) & BlockersMask_Rook[Index];
		uint64_t Key = (Blockers * RookMagics[Index]) >> (64 - RookIndexBits[Index]);
		State->BMVision[Index] = VisionMask_Rook[Index][Key];
	});
}

void MoveGen::ComputeVision_Knight(UChessModel::FBoardState* State)
{
	BitboardUtils::ForEach(State->BMPieces[2], [&](int Index) {
		State->BMVision[Index] = VisionMask_Knight[Index];
	});
}

void MoveGen::ComputeVision_Bishop(UChessModel::FBoardState* State)
{
	BitboardUtils::ForEach(State->BMPieces[3], [&](int Index) {
		uint64_t Blockers = (State->BMBlack | State->BMWhite) & BlockersMask_Bishop[Index];
		uint64_t Key = (Blockers * BishopMagics[Index]) >> (64 - BishopIndexBits[Index]);
		State->BMVision[Index] = VisionMask_Bishop[Index][Key];
	});
}

void MoveGen::ComputeVision_Queen(UChessModel::FBoardState* State)
{
	BitboardUtils::ForEach(State->BMPieces[4], [&](int Index) {
		uint64_t BlockersRook = (State->BMBlack | State->BMWhite) & BlockersMask_Rook[Index];
		uint64_t Key = (BlockersRook * RookMagics[Index]) >> (64 - RookIndexBits[Index]);
		State->BMVision[Index] |= VisionMask_Rook[Index][Key];

		uint64_t BlockersBishop = (State->BMBlack | State->BMWhite) & BlockersMask_Bishop[Index];
		Key = (BlockersBishop * BishopMagics[Index]) >> (64 - BishopIndexBits[Index]);
		State->BMVision[Index] |= VisionMask_Bishop[Index][Key];
	});
}

void MoveGen::ComputeVision_King(UChessModel::FBoardState* State)
{
	BitboardUtils::ForEach(State->BMPieces[5], [&](int Index) {
		State->BMVision[Index] = VisionMask_King[Index];
	});
}