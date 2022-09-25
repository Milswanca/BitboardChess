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

glm::vec3 UChessStatics::IndexToWorld(uchar X, uchar Y)
{
	float Bottom = (SquareSize * -4) + (SquareSize * 0.5f);
	float Left = (SquareSize * -4) + (SquareSize * 0.5f);
	return glm::vec3(Left + (X * SquareSize), Bottom + (Y * SquareSize), 0.0f);
}

bool UChessStatics::WorldToIndex(const glm::vec3& World, uchar& X, uchar& Y)
{
	float Bottom = (SquareSize * -4);
	float Left = (SquareSize * -4);

	X = (uchar)((World.x - Left) / SquareSize);
	Y = (uchar)((World.y - Bottom) / SquareSize);
	return X < 8 && Y < 8;
}