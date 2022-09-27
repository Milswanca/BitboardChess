#pragma once
#include <glm/glm.hpp>

typedef unsigned char uchar;

constexpr uint64_t A1 = (1ULL << 0);
constexpr uint64_t B1 = (1ULL << 1);
constexpr uint64_t C1 = (1ULL << 2);
constexpr uint64_t D1 = (1ULL << 3);
constexpr uint64_t E1 = (1ULL << 4);
constexpr uint64_t F1 = (1ULL << 5);
constexpr uint64_t G1 = (1ULL << 6);
constexpr uint64_t H1 = (1ULL << 7);
constexpr uint64_t A2 = (1ULL << 8);
constexpr uint64_t B2 = (1ULL << 9);
constexpr uint64_t C2 = (1ULL << 10);
constexpr uint64_t D2 = (1ULL << 11);
constexpr uint64_t E2 = (1ULL << 12);
constexpr uint64_t F2 = (1ULL << 13);
constexpr uint64_t G2 = (1ULL << 14);
constexpr uint64_t H2 = (1ULL << 15);
constexpr uint64_t A3 = (1ULL << 16);
constexpr uint64_t B3 = (1ULL << 17);
constexpr uint64_t C3 = (1ULL << 18);
constexpr uint64_t D3 = (1ULL << 19);
constexpr uint64_t E3 = (1ULL << 20);
constexpr uint64_t F3 = (1ULL << 21);
constexpr uint64_t G3 = (1ULL << 22);
constexpr uint64_t H3 = (1ULL << 23);
constexpr uint64_t A4 = (1ULL << 24);
constexpr uint64_t B4 = (1ULL << 25);
constexpr uint64_t C4 = (1ULL << 26);
constexpr uint64_t D4 = (1ULL << 27);
constexpr uint64_t E4 = (1ULL << 28);
constexpr uint64_t F4 = (1ULL << 29);
constexpr uint64_t G4 = (1ULL << 30);
constexpr uint64_t H4 = (1ULL << 31);
constexpr uint64_t A5 = (1ULL << 32);
constexpr uint64_t B5 = (1ULL << 33);
constexpr uint64_t C5 = (1ULL << 34);
constexpr uint64_t D5 = (1ULL << 35);
constexpr uint64_t E5 = (1ULL << 36);
constexpr uint64_t F5 = (1ULL << 37);
constexpr uint64_t G5 = (1ULL << 38);
constexpr uint64_t H5 = (1ULL << 39);
constexpr uint64_t A6 = (1ULL << 40);
constexpr uint64_t B6 = (1ULL << 41);
constexpr uint64_t C6 = (1ULL << 42);
constexpr uint64_t D6 = (1ULL << 43);
constexpr uint64_t E6 = (1ULL << 44);
constexpr uint64_t F6 = (1ULL << 45);
constexpr uint64_t G6 = (1ULL << 46);
constexpr uint64_t H6 = (1ULL << 47);
constexpr uint64_t A7 = (1ULL << 48);
constexpr uint64_t B7 = (1ULL << 49);
constexpr uint64_t C7 = (1ULL << 50);
constexpr uint64_t D7 = (1ULL << 51);
constexpr uint64_t E7 = (1ULL << 52);
constexpr uint64_t F7 = (1ULL << 53);
constexpr uint64_t G7 = (1ULL << 54);
constexpr uint64_t H7 = (1ULL << 55);
constexpr uint64_t A8 = (1ULL << 56);
constexpr uint64_t B8 = (1ULL << 57);
constexpr uint64_t C8 = (1ULL << 58);
constexpr uint64_t D8 = (1ULL << 59);
constexpr uint64_t E8 = (1ULL << 60);
constexpr uint64_t F8 = (1ULL << 61);
constexpr uint64_t G8 = (1ULL << 62);
constexpr uint64_t H8 = (1ULL << 63);

constexpr uint64_t FileA = A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8;
constexpr uint64_t FileB = B1 | B2 | B3 | B4 | B5 | B6 | B7 | B8;
constexpr uint64_t FileC = C1 | C2 | C3 | C4 | C5 | C6 | C7 | C8;
constexpr uint64_t FileD = D1 | D2 | D3 | D4 | D5 | D6 | D7 | D8;
constexpr uint64_t FileE = E1 | E2 | E3 | E4 | E5 | E6 | E7 | E8;
constexpr uint64_t FileF = F1 | F2 | F3 | F4 | F5 | F6 | F7 | F8;
constexpr uint64_t FileG = G1 | G2 | G3 | G4 | G5 | G6 | G7 | G8;
constexpr uint64_t FileH = H1 | H2 | H3 | H4 | H5 | H6 | H7 | H8;

constexpr uint64_t Rank1 = A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1;
constexpr uint64_t Rank2 = A2 | B2 | C2 | D2 | E2 | F2 | G2 | H2;
constexpr uint64_t Rank3 = A3 | B3 | C3 | D3 | E3 | F3 | G3 | H3;
constexpr uint64_t Rank4 = A4 | B4 | C4 | D4 | E4 | F4 | G4 | H4;
constexpr uint64_t Rank5 = A5 | B5 | C5 | D5 | E5 | F5 | G5 | H5;
constexpr uint64_t Rank6 = A6 | B6 | C6 | D6 | E6 | F6 | G6 | H6;
constexpr uint64_t Rank7 = A7 | B7 | C7 | D7 | E7 | F7 | G7 | H7;
constexpr uint64_t Rank8 = A8 | B8 | C8 | D8 | E8 | F8 | G8 | H8;

enum class ERays
{
	North = 0,
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest
};

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
