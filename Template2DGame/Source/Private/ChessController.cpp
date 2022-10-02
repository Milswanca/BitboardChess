#include "ChessController.h"
#include "Core/Engine.h"
#include "Core/Math.h"
#include "Components/CameraComponent.h"

#include "ChessStatics.h"
#include "ChessModel.h"

#include <GLFW/glfw3.h>

void UChessController::Begin()
{
	UObject::Begin();
}

void UChessController::UpdateController(UChessModel* Model)
{
	bool bState = glfwGetMouseButton(UEngine::Get()->GetMainWindow(), 0);

	glm::vec3 WorldMouse = GetCursorWorldPosition();

	int Index;
	if (UChessStatics::WorldToIndex(WorldMouse, Index))
	{
		if (!bLastMouseState && bState)
		{
			MouseDownSquare = Index;
		}
		else if (bLastMouseState && !bState)
		{
			int Move;
			if (Model->FindMove(MouseDownSquare, Index, Move))
			{
				Model->DoMove(Move);
			}
		}
	}

	bLastMouseState = bState;
}

glm::vec3 UChessController::GetCursorWorldPosition() const
{
	double MouseX;
	double MouseY;
	glfwGetCursorPos(UEngine::Get()->GetMainWindow(), &MouseX, &MouseY);

	glm::vec3 Origin;
	glm::vec3 Direction;
	UCameraComponent::GetMain()->DeprojectScreenToWorld(glm::vec3(MouseX, MouseY, 0.0f), Origin, Direction);

	float Depth;
	UMath::RayPlaneIntersection(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), Origin, Direction, Depth);
	return Origin + (Direction * Depth);
}