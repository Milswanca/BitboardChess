#pragma once
#include "Core/Object.h"
#include "ChessTypes.h"
#include <glm/glm.hpp>

class UChessStatics : public UObject
{
protected:
	IMPLEMENT_CONSTRUCTOR(UChessStatics, UObject);

public:
	static const int SquareSize = 128;

	static EChessPieces ExtractPiece(EChessColoredPieces ColoredPiece);
	static EChessColors ExtractColor(EChessColoredPieces ColoredPiece);
	static void ExtractPieceAndColor(EChessColoredPieces ColoredPiece, EChessPieces& Piece, EChessColors& Color);

	static glm::vec3 IndexToWorld(int Square);
	static bool WorldToIndex(const glm::vec3& World, int& Square);
};

