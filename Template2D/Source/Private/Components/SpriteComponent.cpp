#include "Components/SpriteComponent.h"
#include "Components/CameraComponent.h"
#include "Core/Engine.h"
#include "Rendering/Sprite.h"
#include "Rendering/IRendererImmediate.h"

void USpriteComponent::Begin()
{
	UComponent::Begin();

	Sprite = nullptr;
}

void USpriteComponent::Tick(float dt)
{
	UComponent::Tick(dt);

	if (Sprite != nullptr)
	{
		glm::mat4 Model;
		GetLocalToWorld(Model);

		GetImmediateRenderer()->DrawSprite(Sprite, Material, Model);
	}
}

void USpriteComponent::End()
{
	UComponent::End();
}

void USpriteComponent::SetSprite(USprite* InSprite)
{
	Sprite = InSprite;
}

USprite* USpriteComponent::GetSprite() const
{
	return Sprite;
}

void USpriteComponent::SetMaterial(UMaterial* InMaterial)
{
	Material = InMaterial;
}

UMaterial* USpriteComponent::GetMaterial() const
{
	return Material;
}