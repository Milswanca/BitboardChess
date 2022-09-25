#include "Actors/SpriteActor.h"

void ASpriteActor::Begin()
{
	AActor::Begin();

	SpriteComponent = AddComponent<USpriteComponent>();
	SpriteComponent->AttachTo(GetRootComponent());
}