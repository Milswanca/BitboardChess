#include "ChessView.h"
#include "ChessModel.h"
#include "ChessStatics.h"
#include "Rendering/Sprite.h"
#include "Rendering/IRendererImmediate.h"

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>

#include "Rendering/Shader.h"
#include "Rendering/Material.h"

void UChessView::Begin()
{
	WhitePieces[0] = USprite::Create("Game/Sprites/spr_pawn_white.png");
	WhitePieces[1] = USprite::Create("Game/Sprites/spr_rook_white.png");
	WhitePieces[2] = USprite::Create("Game/Sprites/spr_knight_white.png");
	WhitePieces[3] = USprite::Create("Game/Sprites/spr_bishop_white.png");
	WhitePieces[4] = USprite::Create("Game/Sprites/spr_queen_white.png");
	WhitePieces[5] = USprite::Create("Game/Sprites/spr_king_white.png");
	BlackPieces[0] = USprite::Create("Game/Sprites/spr_pawn_black.png");
	BlackPieces[1] = USprite::Create("Game/Sprites/spr_rook_black.png");
	BlackPieces[2] = USprite::Create("Game/Sprites/spr_knight_black.png");
	BlackPieces[3] = USprite::Create("Game/Sprites/spr_bishop_black.png");
	BlackPieces[4] = USprite::Create("Game/Sprites/spr_queen_black.png");
	BlackPieces[5] = USprite::Create("Game/Sprites/spr_king_black.png");

	SpriteBoardLight = USprite::Create("Game/Sprites/spr_square_brown_light.png");
	SpriteBoardDark = USprite::Create("Game/Sprites/spr_square_brown_dark.png");

	WhiteSquare = USprite::Create(UChessStatics::SquareSize, UChessStatics::SquareSize);

	SpriteBoardLight->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);
	SpriteBoardDark->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);

	for (int i = 0; i < 6; ++i)
	{
		WhitePieces[i]->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);
		BlackPieces[i]->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);
	}

	UShader::FCreateShaderParams Params;
	Params.VertexShader = "Engine/Shaders/SpriteVertex.shader";
	Params.FragmentShader = "Engine/Shaders/SpriteFragment.shader";
	UShader* SpriteDefaultShader = UShader::Create(Params);

	SpriteMaterial = UMaterial::Create(SpriteDefaultShader);
}

void UChessView::End()
{

}

void UChessView::RenderBoard(UChessModel* Model)
{
	// Render background
	for (uchar i = 0; i < 64; ++i)
	{
		uchar x = i % 8;
		uchar y = i / 8;
		USprite* Spr = (x + y) % 2 ? SpriteBoardLight : SpriteBoardDark;
		RenderSprite(Spr, i, 0);
	}

	// Render Pieces
	for (uchar i = 0; i < 64; ++i)
	{
		if (Model->GetOwner(i) == 0)
			continue;

		uchar Owner = Model->GetOwner(i);
		USprite** Sprites = Owner == 1 ? WhitePieces : BlackPieces;
		USprite* Spr = Sprites[Model->GetPiece(i)];

		RenderSprite(Spr, i, 1);
	}

	DebugRenderCheckMask(Model);
}

void UChessView::RenderSprite(USprite* Sprite, int Square, int Order, const glm::vec4& Tint)
{
	glm::vec3 WorldPos = UChessStatics::IndexToWorld(Square);
	GetImmediateRenderer()->DrawSprite(Sprite, nullptr, Tint, glm::translate(glm::identity<glm::mat4>(), glm::vec3(WorldPos.x, WorldPos.y, -Order)));
}

void UChessView::DebugRenderCheckMask(UChessModel* Model)
{
	uint64_t CheckMaskWhite = Model->GetState()->BMCheckMaskWhite;
	uint64_t CheckMaskBlack = Model->GetState()->BMCheckMaskBlack;

	for (int i = 0; i < 64; ++i)
	{
		if (CheckMaskWhite != 0xFFFFFFFFFFFFFFFF && (CheckMaskWhite & (1ULL << i)) != 0)
		{
			RenderSprite(WhiteSquare, i, 2, glm::vec4(1.0, 1.0f, 1.0f, 0.5f));
		}

		if (CheckMaskBlack != 0xFFFFFFFFFFFFFFFF && (CheckMaskBlack & (1ULL << i)) != 0)
		{
			RenderSprite(WhiteSquare, i, 2, glm::vec4(0.0, 0.0f, 0.0f, 0.5f));
		}
	}
}