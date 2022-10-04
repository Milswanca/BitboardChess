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
	static void InitSlidingMasks();
	static void InitPathAndDirectionToSquare();

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

	static void GenerateCheckMask(UChessModel::FBoardState* State);
	static void GeneratePinMask(UChessModel::FBoardState* State);

	static void GenerateMoves_Pawn(UChessModel::FBoardState* State);
	static void GenerateMoves_Rook(UChessModel::FBoardState* State);
	static void GenerateMoves_Knight(UChessModel::FBoardState* State);
	static void GenerateMoves_Bishop(UChessModel::FBoardState* State);
	static void GenerateMoves_Queen(UChessModel::FBoardState* State);
	static void GenerateMoves_King(UChessModel::FBoardState* State);
	static void GeneratePieceMoves(UChessModel::FBoardState* State, int Piece, uint64_t PieceBitboard, uint64_t LegalSquares, uint64_t TeamOccupation);

	static void WriteVision_White(UChessModel::FBoardState* State, int Square, uint64_t Vision);
	static void WriteVision_Black(UChessModel::FBoardState* State, int Square, uint64_t Vision);

private:
	static ERays DirectionToSquare[64][64];
	static uint64_t PathToSquare[64][64];

	static uint64_t VisionMask_King[64];
	static uint64_t VisionMask_Knight[64];
	static uint64_t VisionMask_Rook[64][4096];
	static uint64_t VisionMask_Bishop[64][1024];

	static uint64_t SlidingPieceMasks[6][64];

	// Rook Mask for all squares (without piece blocking) for all visible squares
	static const uint64_t RookMagics[64];
	static const uint64_t BishopMagics[64];

	static uint64_t RookMagicsMask[64];
	static uint64_t BishopMagicsMask[64];

	static const int RookIndexBits[64];
	static const int BishopIndexBits[64];
};

