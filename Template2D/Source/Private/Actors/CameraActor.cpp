#include "Actors/CameraActor.h"

void ACameraActor::Begin()
{
	AActor::Begin();

	CameraComponent = AddComponent<UCameraComponent>();
	CameraComponent->AttachTo(GetRootComponent());
}