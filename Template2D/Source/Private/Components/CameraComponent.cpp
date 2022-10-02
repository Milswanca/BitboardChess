#include "Components/CameraComponent.h"
#include "Core/Engine.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

UCameraComponent* UCameraComponent::MainCamera = nullptr;

void UCameraComponent::Begin()
{
	UComponent::Begin();

	bUseWindowAspect = true;
	ProjectionType = EProjectionTypes::Perspective;
	Near = 0.03f;
	Far = 1000.0f;
	FOV = 45.0f;
	Aspect = 1.0f;
	OrthoWidth = 300;
	RefreshProjectionView();
}

void UCameraComponent::Tick(float dt)
{
	UComponent::Tick(dt);
	RefreshProjectionView();
}

void UCameraComponent::End()
{
	UComponent::End();
}

void UCameraComponent::SetAsMain()
{
	MainCamera = this;
}

UCameraComponent* UCameraComponent::GetMain()
{
	return MainCamera;
}

void UCameraComponent::SetUseWindowAspect(bool bInUseWindowAspect)
{
	bUseWindowAspect = bInUseWindowAspect;
	RefreshProjectionView();
}

void UCameraComponent::SetProjectionType(EProjectionTypes Type)
{
	ProjectionType = Type;
	RefreshProjectionView();
}

void UCameraComponent::SetFOV(float NewFOV)
{
	FOV = NewFOV;
	RefreshProjectionView();
}

void UCameraComponent::SetNear(float NewNear)
{
	Near = NewNear;
	RefreshProjectionView();
}

void UCameraComponent::SetFar(float NewFar)
{
	Far = NewFar;
	RefreshProjectionView();
}

void UCameraComponent::SetOrthoSize(float Width, float Height)
{
	OrthoWidth = Width;
	OrthoHeight = Height;
	RefreshProjectionView();
}

void UCameraComponent::SetOrthoFitMode(EOrthoFitMode FitMode)
{
	OrthoFitMode = FitMode;
	RefreshProjectionView();
}

void UCameraComponent::DeprojectScreenToWorld(const glm::vec3& Screen, glm::vec3& Location, glm::vec3& Direction)
{
	float HalfScreenWidth = UEngine::WindowWidth * 0.5f;
	float HalfScreenHeight = UEngine::WindowHeight * 0.5f;

	glm::mat4 InvMat = glm::inverse(ProjectionView);
	glm::vec4 Near = glm::vec4((Screen.x - HalfScreenWidth) / HalfScreenWidth, -1.0f * (Screen.y - HalfScreenHeight) / HalfScreenHeight, -1.0f, 1.0f);
	glm::vec4 Far = glm::vec4((Screen.x - HalfScreenWidth) / HalfScreenWidth, -1.0f * (Screen.y - HalfScreenHeight) / HalfScreenHeight, 1.0f, 1.0);
	glm::vec4 NearResult = InvMat * Near;
	glm::vec4 FarResult = InvMat * Far;
	NearResult /= NearResult.w;
	FarResult /= FarResult.w;

	Location = NearResult;
	Direction = glm::normalize(glm::vec3(FarResult - NearResult));
}

bool UCameraComponent::GetUseWindowAspect() const
{
	return bUseWindowAspect;
}

EProjectionTypes UCameraComponent::GetProjectionType() const
{
	return ProjectionType;
}

float UCameraComponent::GetFOV() const
{
	return FOV;
}

float UCameraComponent::GetNear() const
{
	return Near;
}

float UCameraComponent::GetFar() const
{
	return Far;
}

glm::mat4 UCameraComponent::GetProjection() const
{
	return Projection;
}

glm::mat4 UCameraComponent::GetView() const
{
	return View;
}

glm::mat4 UCameraComponent::GetProjectionView() const
{
	return ProjectionView;
}

void UCameraComponent::RefreshProjectionView()
{
	float Aspect = (float)UEngine::WindowWidth / (float)UEngine::WindowHeight;

	if (ProjectionType == EProjectionTypes::Perspective)
	{
		Projection = glm::perspective(GetFOV(), Aspect, GetNear(), GetFar());
	}
	else
	{
		float OrthoAspect = OrthoWidth / OrthoHeight;
		float FinalWidth = 0.0f;
		float FinalHeight = 0.0f;

		switch (OrthoFitMode)
		{
		case EOrthoFitMode::Stretch:
			FinalWidth = OrthoWidth;
			FinalHeight = OrthoHeight;
			break;

		case EOrthoFitMode::FlexibleWidth:
			FinalWidth = OrthoHeight * Aspect;
			FinalHeight = OrthoHeight;
			break;

		case EOrthoFitMode::FlexibleHeight:
			FinalWidth = OrthoWidth;
			FinalHeight = OrthoWidth * (1.0f / Aspect);
			break;

		case EOrthoFitMode::Fit:
			if (Aspect < OrthoAspect)
			{
				FinalWidth = OrthoWidth;
				FinalHeight = OrthoWidth * (1.0f / Aspect);
			}
			else
			{
				FinalWidth = OrthoHeight * Aspect;
				FinalHeight = OrthoHeight;
			}
			break;
		}

		Projection = glm::ortho(-FinalWidth * 0.5f, FinalWidth * 0.5f, -FinalHeight * 0.5f, FinalHeight * 0.5f, GetNear(), GetFar());
	}

	glm::vec3 Forward = GetForward();
	glm::vec3 Position = GetPosition();
	glm::vec3 Up = GetUp();
	View = glm::lookAt(GetPosition(), GetPosition() + (GetForward() * 100.0f), GetUp());
	ProjectionView = Projection * View;
}