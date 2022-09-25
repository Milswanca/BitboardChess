#pragma once
typedef unsigned char uchar;

enum class EChessColors
{
	White,
	Black
};

enum class EChessPieces : uchar
{
	None = 0,
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King,
	COUNT,
};

enum class EChessColoredPieces : uchar
{
	// Bytes 1 - 4 = Piece
	// Fifth Byte = 1 if Black
	None = 0,
	WhitePawn = static_cast<uchar>(EChessPieces::Pawn),
	WhiteRook = static_cast<uchar>(EChessPieces::Rook),
	WhiteKnight = static_cast<uchar>(EChessPieces::Knight),
	WhiteBishop = static_cast<uchar>(EChessPieces::Bishop),
	WhiteQueen = static_cast<uchar>(EChessPieces::Queen),
	WhiteKing = static_cast<uchar>(EChessPieces::King),
	BlackPawn = (1 << 4) | static_cast<uchar>(EChessPieces::Pawn),
	BlackRook = (1 << 4) | static_cast<uchar>(EChessPieces::Rook),
	BlackKnight = (1 << 4) | static_cast<uchar>(EChessPieces::Knight),
	BlackBishop = (1 << 4) | static_cast<uchar>(EChessPieces::Bishop),
	BlackQueen = (1 << 4) | static_cast<uchar>(EChessPieces::Queen),
	BlackKing = (1 << 4) | static_cast<uchar>(EChessPieces::King),
};