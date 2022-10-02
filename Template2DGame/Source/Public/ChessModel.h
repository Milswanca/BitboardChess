#pragma once
#include "Core/Object.h"
#include "ChessTypes.h"
#include <array>
#include <glm/glm.hpp>

class UChessModel : public UObject
{
public:
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
		uint64_t BMVisionWhite;
		uint64_t BMVisionBlack;
		uint64_t BMCheckMaskWhite;
		uint64_t BMCheckMaskBlack;

		// Colors
		uint64_t BMWhite;
		uint64_t BMBlack;

		// Pins
		uint64_t BMHPin;
		uint64_t BMVPin;
		uint64_t BMD1Pin;
		uint64_t BMD2Pin;

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
	FBoardState* State;
};