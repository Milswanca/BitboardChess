#pragma warning(disable:4146)
#include "ChessModel.h"
#include "ChessStatics.h"

#include <glm/gtc/integer.hpp>

const uint64_t UChessModel::AFile = 0b0000000100000001000000010000000100000001000000010000000100000001;
const uint64_t UChessModel::HFile = 0b1000000010000000100000001000000010000000100000001000000010000000;
const uint64_t UChessModel::Rank2 = 0b0000000000000000000000000000000000000000000000001111111100000000;
const uint64_t UChessModel::Rank7 = 0b0000000011111111000000000000000000000000000000000000000000000000;

int move_encode(uchar source, uchar target, uchar piece, uchar promotion)
{
	return (source) | (target << 6) | (piece << 12) | (promotion << 16);
}

uchar move_get_source(int move)
{
	return move & 0x3f;
}

uchar move_get_target(int move)
{
	return (move & 0xfc0) >> 6;
}

uchar move_get_piece(int move)
{
	return (move & 0xf000) >> 12;
}

uchar move_get_promotion(int move)
{
	return (move & 0xf0000) >> 16;
}

void pop_bit(uint64_t& bitboard, uchar index)
{
	(bitboard) &= ~(1ULL << (index));
}

void set_bit(uint64_t& bitboard, uchar index)
{
	bitboard |= (1ULL << index);
}

bool get_bit(uint64_t bitboard, uchar index)
{
	return ((bitboard >> index) & 1);
}

void move_bit(uint64_t& bitboard, uchar from, uchar to)
{
	// Clear the to bit
	bitboard &= ~(1ULL << to);

	// Set the to bit to 1 if it is on in from
	bitboard |= ((uint64_t)get_bit(bitboard, from) << to);

	// Clear the from bit
	bitboard &= ~(1ULL << from);
}

uchar count_bits(uint64_t bitboard)
{
	int count = 0;

	while (bitboard)
	{
		count++;
		bitboard &= bitboard - 1;
	}

	return count;
}

uchar get_ls_bit_index(uint64_t bitboard)
{
	if (bitboard)
	{
		return (uchar)count_bits((bitboard & -bitboard) - 1);
	}
	else
	{
		return 255;
	}
}

UChessModel::FBoardState::FBoardState()
{
	BMPieces[0] = 0b0000000011111111000000000000000000000000000000001111111100000000;
	BMPieces[1] = 0b1000000100000000000000000000000000000000000000000000000010000001;
	BMPieces[2] = 0b0100001000000000000000000000000000000000000000000000000001000010;
	BMPieces[3] = 0b0010010000000000000000000000000000000000000000000000000000100100;
	BMPieces[4] = 0b0001000000000000000000000000000000000000000000000000000000010000;
	BMPieces[5] = 0b0000100000000000000000000000000000000000000000000000000000001000;

	BMWhite = 0b0000000000000000000000000000000000000000000000001111111111111111;
	BMBlack = 0b1111111111111111000000000000000000000000000000000000000000000000;
	BMCheck = 0b0000000000000000000000000000000000000000000000000000000000000000;
	BMHPin = 0b0000000000000000000000000000000000000000000000000000000000000000;
	BMVPin = 0b0000000000000000000000000000000000000000000000000000000000000000;
	BMD1Pin = 0b0000000000000000000000000000000000000000000000000000000000000000;
	BMD2Pin = 0b0000000000000000000000000000000000000000000000000000000000000000;

	NumMoves = 0;
	Turn = EChessColors::White;
}

UChessModel::FBoardState::~FBoardState()
{

}

void UChessModel::Begin()
{
	UObject::Begin();

	State = new	FBoardState();
	CalculateMoves();
}

void UChessModel::End()
{
	UObject::End();

	delete State;
	State = nullptr;
}

bool UChessModel::DoMove(int Move)
{
	uchar Source = move_get_source(Move);
	uchar Target = move_get_target(Move);
	uchar Piece = move_get_piece(Move);

	for (uchar i = 0; i < 6; ++i)
	{
		pop_bit(State->BMPieces[i], Target);
	}

	move_bit(State->BMPieces[Piece], Source, Target);
	move_bit(State->BMWhite, Source, Target);
	move_bit(State->BMBlack, Source, Target);

	CalculateMoves();

	return true;
}

bool UChessModel::FindMove(uchar From, uchar To, int& Move) const
{
	int MoveMask = From | (To << 6);

	for (int i = 0; i < State->NumMoves; ++i)
	{
		if ((State->Moves[i] & 0xfff) == MoveMask)
		{
			Move = State->Moves[i];
			return true;
		}
	}

	return false;
}

void UChessModel::CalculateVision()
{
	for (uchar i = 0; i < 64; ++i)
	{
		State->BMVision[i] = 0;
	}

	CalculateVision_Pawn();
}

void UChessModel::CalculateVision_Pawn()
{
	uint64_t WhitePawns = State->BMPieces[0] & State->BMWhite;
	uint64_t WhitePawnsRank2 = WhitePawns & Rank2;
	uint64_t WhitePawnsNotRank2 = WhitePawns & ~(Rank2);
	uint64_t WhitePawnsNotAFile = WhitePawns & ~(AFile);
	uint64_t WhitePawnsNotHFile = WhitePawns & ~(HFile);

	// Forward Moves - White
	while (WhitePawnsNotRank2)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(WhitePawnsNotRank2);
		if (!get_bit(State->BMBlack | State->BMWhite, Source + 8))
		{
			set_bit(PieceVision, Source + 8);
		}

		pop_bit(WhitePawnsNotRank2, Source);
		State->BMVision[Source] |= PieceVision;
	}

	// Double Moves - White
	while (WhitePawnsRank2)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(WhitePawnsRank2);
		if (!get_bit(State->BMBlack | State->BMWhite, Source + 8))
		{
			set_bit(PieceVision, Source + 8);
			if (!get_bit(State->BMBlack | State->BMWhite, Source + 16))
			{
				set_bit(PieceVision, Source + 16);
			}
		}

		pop_bit(WhitePawnsRank2, Source);
		State->BMVision[Source] |= PieceVision;
	}

	// Attack Left Moves - White
	while (WhitePawnsNotAFile)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(WhitePawnsNotAFile);
		if (get_bit(State->BMBlack, Source + 7))
		{
			set_bit(PieceVision, Source + 7);
		}

		pop_bit(WhitePawnsNotAFile, Source);
		State->BMVision[Source] |= PieceVision;
	}

	// Attack Right Moves - White
	while (WhitePawnsNotHFile)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(WhitePawnsNotHFile);
		if (get_bit(State->BMBlack, Source + 9))
		{
			set_bit(PieceVision, Source + 9);
		}

		pop_bit(WhitePawnsNotHFile, Source);
		State->BMVision[Source] |= PieceVision;
	}

	uint64_t BlackPawns = State->BMPieces[0] & State->BMBlack;
	uint64_t BlackPawnsRank7 = BlackPawns & Rank7;
	uint64_t BlackPawnsNotRank7 = BlackPawns & ~(Rank7);
	uint64_t BlackPawnsNotAFile = BlackPawns & ~(AFile);
	uint64_t BlackPawnsNotHFile = BlackPawns & ~(HFile);

	// Forward Moves - Black
	while (BlackPawnsNotRank7)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(BlackPawnsNotRank7);
		if (!get_bit(State->BMBlack | State->BMWhite, Source - 8))
		{
			set_bit(PieceVision, Source - 8);
		}

		pop_bit(BlackPawnsNotRank7, Source);
		State->BMVision[Source] |= PieceVision;
	}

	// Double Moves - Black
	while (BlackPawnsRank7)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(BlackPawnsRank7);
		if (!get_bit(State->BMBlack | State->BMWhite, Source - 8))
		{
			set_bit(PieceVision, Source - 8);
			if (!get_bit(State->BMBlack | State->BMWhite, Source - 16))
			{
				set_bit(PieceVision, Source - 16);
			}
		}

		pop_bit(BlackPawnsRank7, Source);
		State->BMVision[Source] |= PieceVision;
	}

	// Attack Left Moves - Black
	while (BlackPawnsNotAFile)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(BlackPawnsNotAFile);
		if (get_bit(State->BMWhite, Source - 9))
		{
			set_bit(PieceVision, Source - 9);
		}

		pop_bit(BlackPawnsNotAFile, Source);
		State->BMVision[Source] |= PieceVision;
	}

	// Attack Right Moves - Black
	while (BlackPawnsNotHFile)
	{
		uint64_t PieceVision = 0u;
		uchar Source = get_ls_bit_index(BlackPawnsNotHFile);
		if (get_bit(State->BMWhite, Source - 7))
		{
			set_bit(PieceVision, Source - 7);
		}

		pop_bit(BlackPawnsNotHFile, Source);
		State->BMVision[Source] |= PieceVision;
	}
}

void UChessModel::CalculateMoves()
{
	State->NumMoves = 0;
	CalculateVision();

	for (uchar i = 0; i < 64; ++i)
	{
		uint64_t Bitboard = State->BMVision[i];

		while (Bitboard)
		{
			uchar Source = i;
			uchar Target = get_ls_bit_index(Bitboard);
			uchar Piece = GetPiece(Source);

			State->Moves[State->NumMoves++] = move_encode(Source, Target, Piece, 0);

			pop_bit(Bitboard, Target);
		}
	}
}

uchar UChessModel::GetPiece(uchar Square) const
{
	uchar Piece = 0;
	for (uchar i = 0; i < 6; ++i)
	{
		Piece |= (get_bit(State->BMPieces[i], Square) << i);
	}

	return glm::log2(Piece);
}

uchar UChessModel::GetOwner(uchar Square) const
{
	uchar Owner = 0;
	Owner = (get_bit(State->BMBlack, Square) << 2) | (get_bit(State->BMWhite, Square) << 1) | (get_bit(~(State->BMWhite | State->BMBlack), Square) << 0);
	return glm::log2(Owner);
}