#include "ChessStatics.h"

EChessPieces UChessStatics::ExtractPiece(EChessColoredPieces ColoredPiece)
{
	return static_cast<EChessPieces>(0x0f & static_cast<uchar>(ColoredPiece));
}

EChessColors UChessStatics::ExtractColor(EChessColoredPieces ColoredPiece)
{
	return ((0xf0 & static_cast<uchar>(ColoredPiece)) >> 4) != 0 ? EChessColors::Black : EChessColors::White;
}

void UChessStatics::ExtractPieceAndColor(EChessColoredPieces ColoredPiece, EChessPieces& Piece, EChessColors& Color)
{
	Piece = ExtractPiece(ColoredPiece);
	Color = ExtractColor(ColoredPiece);
}

glm::vec3 UChessStatics::IndexToWorld(int Square)
{
	float Bottom = (SquareSize * -4) + (SquareSize * 0.5f);
	float Left = (SquareSize * -4) + (SquareSize * 0.5f);
	return glm::vec3(Left + ((Square % 8) * SquareSize), Bottom + ((Square / 8) * SquareSize), 0.0f);
}

bool UChessStatics::WorldToIndex(const glm::vec3& World, int& Square)
{
	float Bottom = (SquareSize * -4);
	float Left = (SquareSize * -4);

	int X = ((World.x - Left) / SquareSize);
	int Y = ((World.y - Bottom) / SquareSize);
	Square = X + (Y * 8);

	return X < 8 && Y < 8;
}