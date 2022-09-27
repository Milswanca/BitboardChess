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

private:
	FBoardState* State;
};