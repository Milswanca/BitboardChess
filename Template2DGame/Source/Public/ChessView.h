#pragma once
#include "Core/Object.h"
#include "ChessTypes.h"
#include <unordered_map>

class USprite;
class UChessModel;
class UMaterial;

class UChessView : public UObject
{
protected:
	IMPLEMENT_CONSTRUCTOR(UChessView, UObject);

	virtual void Begin() override;
	virtual void End() override;

public:
	void RenderBoard(UChessModel* Model);
	void DebugRenderCheckMask(UChessModel* Model);

private:
	void RenderSprite(USprite* Sprite, int Square, int Order, const glm::vec4& Tint = glm::vec4(1.0f));

	USprite* WhiteSquare;
	UMaterial* SpriteMaterial;
	USprite* WhitePieces[6];
	USprite* BlackPieces[6];
	USprite* SpriteBoardLight;
	USprite* SpriteBoardDark;
};

