#include "ChessView.h"
#include "ChessModel.h"
#include "ChessStatics.h"
#include "Rendering/Sprite.h"
#include "Rendering/IRendererImmediate.h"

#include "glm/glm.hpp"
#include <glm/gtx/transform.hpp>

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

	SpriteBoardLight->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);
	SpriteBoardDark->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);

	for (int i = 0; i < 6; ++i)
	{
		WhitePieces[i]->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);
		BlackPieces[i]->OverrideDimensions(UChessStatics::SquareSize, UChessStatics::SquareSize);
	}
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
		glm::vec3 WorldPos = UChessStatics::IndexToWorld(x, y);
		GetImmediateRenderer()->DrawSprite(Spr, nullptr, glm::translate(glm::identity<glm::mat4>(), glm::vec3(WorldPos.x, WorldPos.y, -1.0f)));
	}

	// Render Pieces
	for (uchar i = 0; i < 64; ++i)
	{
		uchar x = i % 8;
		uchar y = i / 8;

		if (Model->GetOwner(i) == 0)
			continue;

		uchar Owner = Model->GetOwner(i);
		USprite** Sprites = Owner == 1 ? WhitePieces : BlackPieces;
		USprite* Spr = Sprites[Model->GetPiece(i)];

		glm::vec3 WorldPos = UChessStatics::IndexToWorld(x, y);
		GetImmediateRenderer()->DrawSprite(Spr, nullptr, glm::translate(glm::identity<glm::mat4>(), WorldPos));
	}
}