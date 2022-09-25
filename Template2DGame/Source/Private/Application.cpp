#include "Application.h"
#include "Actors/SpriteActor.h"
#include "Actors/CameraActor.h"
#include "Core/World.h"
#include "ChessStatics.h"

#include "ChessModel.h"
#include "ChessView.h"
#include "ChessController.h"

void AApplication::Begin()
{
	AActor::Begin();

	Camera = GetWorld()->SpawnActor<ACameraActor>();
	Camera->GetCameraComponent()->SetProjectionType(EProjectionTypes::Orthographic);
	Camera->GetCameraComponent()->SetAsMain();
	Camera->GetCameraComponent()->TranslateLocal(GetForward() * -1.0f);
	Camera->GetCameraComponent()->SetOrthoSize(8 * UChessStatics::SquareSize, 8 * UChessStatics::SquareSize);
	Camera->GetCameraComponent()->SetOrthoFitMode(EOrthoFitMode::Fit);

	Model = GetEngine()->NewObject<UChessModel>();
	View = GetEngine()->NewObject<UChessView>();
	Controller = GetEngine()->NewObject<UChessController>();
}

void AApplication::Tick(float dt)
{
	AActor::Tick(dt);

	Controller->UpdateController(Model);
	View->RenderBoard(Model);
}

void AApplication::End()
{
	AActor::End();
}