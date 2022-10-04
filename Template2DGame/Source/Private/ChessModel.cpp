#include "ChessModel.h"
#include "ChessStatics.h"
#include "BitboardUtils.h"
#include "MoveGen.h"
#include "Rays.h"
#include <iostream>

#include <functional>
#include <glm/gtc/integer.hpp>

UChessModel::FBoardState::FBoardState()
{
	BMPieces[0] = Rank2 | Rank7;
	BMPieces[1] = A1 | H1 | A8 | H8;
	BMPieces[2] = B1 | G1 | B8 | G8;
	BMPieces[3] = C1 | F1 | C8 | F8;
	BMPieces[4] = D1 | D8;
	BMPieces[5] = E1 | E8;
	BMSlidingPieces = A1 | C1 | D1 | F1 | H1 | A8 | C8 | D8 | F8 | H8;

	CheckMask = 0ULL;
	White.HVPinMask = 0ULL;
	White.D12PinMask = 0ULL;
	White.PinMaskRefs[static_cast<int>(ERays::North)] = &White.HVPinMask;
	White.PinMaskRefs[static_cast<int>(ERays::South)] = &White.HVPinMask;
	White.PinMaskRefs[static_cast<int>(ERays::East)] = &White.HVPinMask;
	White.PinMaskRefs[static_cast<int>(ERays::West)] = &White.HVPinMask;
	White.PinMaskRefs[static_cast<int>(ERays::NorthEast)] = &White.D12PinMask;
	White.PinMaskRefs[static_cast<int>(ERays::SouthEast)] = &White.D12PinMask;
	White.PinMaskRefs[static_cast<int>(ERays::SouthWest)] = &White.D12PinMask;
	White.PinMaskRefs[static_cast<int>(ERays::NorthWest)] = &White.D12PinMask;

	White.Occupation = Rank1 | Rank2;
	White.Vision = 0ULL;

	Black.HVPinMask = 0ULL;
	Black.D12PinMask = 0ULL;
	Black.PinMaskRefs[static_cast<int>(ERays::North)] = &Black.HVPinMask;
	Black.PinMaskRefs[static_cast<int>(ERays::South)] = &Black.HVPinMask;
	Black.PinMaskRefs[static_cast<int>(ERays::East)] = &Black.HVPinMask;
	Black.PinMaskRefs[static_cast<int>(ERays::West)] = &Black.HVPinMask;
	Black.PinMaskRefs[static_cast<int>(ERays::NorthEast)] = &Black.D12PinMask;
	Black.PinMaskRefs[static_cast<int>(ERays::SouthEast)] = &Black.D12PinMask;
	Black.PinMaskRefs[static_cast<int>(ERays::SouthWest)] = &Black.D12PinMask;
	Black.PinMaskRefs[static_cast<int>(ERays::NorthWest)] = &Black.D12PinMask;

	Black.Occupation = Rank7 | Rank8;
	Black.Vision = 0ULL;

	NumMoves = 0;
	Turn = EChessColors::White;
}

UChessModel::FBoardState::~FBoardState()
{

}

int UChessModel::FBoardState::GetPiece(int Square) const
{
	uchar Piece = 0;
	for (uchar i = 0; i < 6; ++i)
	{
		Piece |= (BitboardUtils::Get(BMPieces[i], Square) << i);
	}

	return glm::log2(Piece);
}

int UChessModel::FBoardState::GetOwner(int Square) const
{
	uchar Owner = 0;

	Owner = 
		(BitboardUtils::Get(Black.Occupation, Square) << 2) | 
		(BitboardUtils::Get(White.Occupation, Square) << 1) | 
		(BitboardUtils::Get(~(White.Occupation | Black.Occupation), Square) << 0);

	return glm::log2(Owner);
}

void UChessModel::Begin()
{
	UObject::Begin();

	Rays::Init();
	MoveGen::Init();

	State = new	FBoardState();
	PlayerTurn = 1;
	GenerateMoves();
}

void UChessModel::End()
{
	UObject::End();

	delete State;
	State = nullptr;
}

bool UChessModel::DoMove(int Move)
{
	int Source;
	int Target;
	int Piece;
	int Promotion;
	MoveGen::DecodeMove(Move, Source, Target, Piece, Promotion);

	int Owner = GetOwner(Source);
	if (Owner != PlayerTurn)
	{
		return false;
	}

	for (uchar i = 0; i < 6; ++i)
	{
		BitboardUtils::Pop(State->BMPieces[i], Target);
	}

	BitboardUtils::Move(State->BMPieces[Piece], Source, Target);
	BitboardUtils::Move(State->BMSlidingPieces, Source, Target);
	BitboardUtils::Move(State->White.Occupation, Source, Target);
	BitboardUtils::Move(State->Black.Occupation, Source, Target);

	// Swap to other player
	PlayerTurn = PlayerTurn == 1 ? 2 : 1;

	GenerateMoves();
	
	return true;
}

bool UChessModel::FindMove(uchar From, uchar To, int& Move) const
{
	int MoveMask = From | (To << 6);

	for (int i = 0; i < State->NumMoves; ++i)
	{
		if ((State->Moves[i] & 0xFFF) == MoveMask)
		{
			Move = State->Moves[i];
			return true;
		}
	}

	return false;
}

void UChessModel::GenerateMoves()
{
	MoveGen::GenerateMoves(State);
}

int UChessModel::GetPiece(int Square) const
{
	return State->GetPiece(Square);
}

int UChessModel::GetOwner(int Square) const
{
	return State->GetOwner(Square);
}

UChessModel::FBoardState* UChessModel::GetState() const
{
	return State;
}