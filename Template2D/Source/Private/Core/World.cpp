#include "Core/World.h"

void UWorld::Begin()
{
	ConstThis = this;
}

void UWorld::TickWorld(float dt)
{
	for (int i = 0; i < Actors.size(); ++i)
	{
		Actors[i]->Tick(dt);
	}
}

UWorld* UWorld::GetWorld() const
{
	return ConstThis;
}