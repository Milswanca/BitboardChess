#pragma once
#include "Core/Object.h"
#include "ChessTypes.h"
#include <array>
#include <glm/glm.hpp>

class UChessModel : public UObject
{
public:
	struct FSideState
	{
	public:
		uint64_t Vision;
		uint64_t Occupation;

		uint64_t HVPinMask;
		uint64_t D12PinMask;
		uint64_t* PinMaskRefs[8];
	};

	struct FBoardState
	{
		FBoardState();
		~FBoardState();

	public:
		EChessColors Turn;

		int Moves[64];
		unsigned int NumMoves;

		// Pieces
		uint64_t BMPieces[6];
		uint64_t BMSlidingPieces;

		// Vision
		uint64_t Targeting[64];
		uint64_t TargetedBy[64];

		// Check Mask - we only need one because if you're in check it has to be your turn
		uint64_t CheckMask;

		// Color State
		FSideState White;
		FSideState Black;

		int GetPiece(int Square) const;
		int GetOwner(int Square) const;
	};

protected:
	IMPLEMENT_CONSTRUCTOR(UChessModel, UObject);

	virtual void Begin() override;
	virtual void End() override;

public:
	const int SquareSize = 128;

	bool DoMove(int Move);
	bool FindMove(uchar From, uchar To, int& Move) const;

	void GenerateMoves();
	int GetPiece(int Square) const;
	int GetOwner(int Square) const;

	FBoardState* GetState() const;

private:
	int PlayerTurn;
	FBoardState* State;
};