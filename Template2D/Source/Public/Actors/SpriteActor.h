#pragma once
#include "Core/Actor.h"
#include "Components/SpriteComponent.h"

class ASpriteActor : public AActor
{
protected:
	IMPLEMENT_CONSTRUCTOR(ASpriteActor, AActor);

	virtual void Begin() override;

public:
	inline USpriteComponent* GetSpriteComponent() const { return SpriteComponent; }

private:
	USpriteComponent* SpriteComponent;
};

