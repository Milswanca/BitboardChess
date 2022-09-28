#pragma once
#include <glm/glm.hpp>
#include "ChessModel.h"

#include <functional>

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

	static void ComputeVision_Pawn(UChessModel::FBoardState* State);
	static void ComputeVision_Rook(UChessModel::FBoardState* State);
	static void ComputeVision_Knight(UChessModel::FBoardState* State);
	static void ComputeVision_Bishop(UChessModel::FBoardState* State);
	static void ComputeVision_Queen(UChessModel::FBoardState* State);
	static void ComputeVision_King(UChessModel::FBoardState* State);

private:
	static uint64_t VisionMask_King[64];
	static uint64_t VisionMask_Knight[64];
	static uint64_t VisionMask_Rook[64][4096];
	static uint64_t VisionMask_Bishop[64][1024];

	static uint64_t BlockersMask_Rook[64];
	static uint64_t BlockersMask_Bishop[64];

	// Rook Mask for all squares (without piece blocking) for all visible squares
	static const uint64_t RookMagics[64];
	static const uint64_t BishopMagics[64];

	static const int RookIndexBits[64];
	static const int BishopIndexBits[64];
};

