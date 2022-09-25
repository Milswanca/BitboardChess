#pragma once
#include "Core/Object.h"
#include "ChessTypes.h"
#include <unordered_map>

class USprite;
class UChessModel;

class UChessView : public UObject
{
protected:
	IMPLEMENT_CONSTRUCTOR(UChessView, UObject);

	virtual void Begin() override;
	virtual void End() override;

public:
	void RenderBoard(UChessModel* Model);

private:
	USprite* WhitePieces[6];
	USprite* BlackPieces[6];
	USprite* SpriteBoardLight;
	USprite* SpriteBoardDark;
};

