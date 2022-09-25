#pragma once
#include "Core/Actor.h"
#include "Components/CameraComponent.h"

class ACameraActor : public AActor
{
protected:
	IMPLEMENT_CONSTRUCTOR(ACameraActor, AActor);

	virtual void Begin() override;

public:
	inline UCameraComponent* GetCameraComponent() const { return CameraComponent; }

private:
	UCameraComponent* CameraComponent;
};

