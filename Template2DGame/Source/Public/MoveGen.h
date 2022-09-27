#pragma once
#include <glm/glm.hpp>
#include "ChessModel.h"

class MoveGen
{
public:
	static void Init();
	static void GenerateMoves(UChessModel::FBoardState* State);

	static int EncodeMove(int source, int target, int piece, int promotion);
	static void DecodeMove(int Move, int& Source, int& Target, int& Piece, int& Promotion);

private:
	static void InitVision_Knight();
	static void InitVision_Rook();
	static void InitVision_Bishop();
	static void InitVision_King();

	static uint64_t ComputeRookAttacks(int Square, uint64_t Blockers);

	static uint64_t GetVision_Pawn(int Square, UChessModel::FBoardState* State);
	static uint64_t GetVision_Knight(int Square, UChessModel::FBoardState* State);
	static uint64_t GetVision_Rook(int Square, UChessModel::FBoardState* State);

private:
	static uint64_t KnightVisionMask[64];
	static uint64_t RookVisionMask[64][4096];
	static uint64_t BishopVisionMask[64][1024];
	static uint64_t KingVisionMask[64];

	// Rook Mask for all squares (without piece blocking) for all visible squares
	static uint64_t RookFullMovementMask[64];

	static const uint64_t RookMagics[64];
	static const uint64_t BishopMagics[64];

	static const int RookIndexBits[64];
	static const int BishopIndexBits[64];
};

