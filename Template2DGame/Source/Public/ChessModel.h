#pragma once
#include "Core/Object.h"
#include "ChessTypes.h"
#include <glm/glm.hpp>

class UChessModel : public UObject
{
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
		uint64_t BMVision[64];

		// Colors
		uint64_t BMWhite;
		uint64_t BMBlack;

		// Checks
		uint64_t BMCheck;

		// Pins
		uint64_t BMHPin;
		uint64_t BMVPin;
		uint64_t BMD1Pin;
		uint64_t BMD2Pin;
	};

protected:
	IMPLEMENT_CONSTRUCTOR(UChessModel, UObject);

	virtual void Begin() override;
	virtual void End() override;

public:
	const int SquareSize = 128;

	bool DoMove(int Move);
	bool FindMove(uchar From, uchar To, int& Move) const;

	void CalculateMoves();
	uchar GetPiece(uchar Square) const;
	uchar GetOwner(uchar Square) const;
	
private:
	static const uint64_t AFile;
	static const uint64_t HFile;
	static const uint64_t Rank2;
	static const uint64_t Rank7;

	void CalculateVision();
	void CalculateVision_Pawn();

	FBoardState* State;
};