#include "Rendering/Sprite.h"
#include "Rendering/Texture2D.h"
#include "Core/Engine.h"

#include <glm/ext/matrix_transform.hpp>

USprite* USprite::Create(const std::string& File)
{
	USprite* Sprite = UEngine::Get()->NewObject<USprite>();
	Sprite->Texture = UTexture2D::Create(File);
	return Sprite;
}

void USprite::Begin()
{
	UObject::Begin();

	OverrideWidth = 0;
	OverrideHeight = 0;
	bOverrideDimensions = false;
	Texture = nullptr;
}

void USprite::OverrideDimensions(unsigned int InWidth, unsigned int InHeight)
{
	OverrideWidth = InWidth;
	OverrideHeight = InHeight;
	bOverrideDimensions = true;
}

void USprite::GetDimensions(unsigned int& OutWidth, unsigned int& OutHeight) const
{
	if (bOverrideDimensions)
	{
		OutWidth = OverrideWidth;
		OutHeight = OverrideHeight;
	}
	else
	{
		OutWidth = Texture->GetTextureData()->Width;
		OutHeight = Texture->GetTextureData()->Height;
	}
}

glm::mat4 USprite::GetScaleMatrix() const
{
	unsigned int Width;
	unsigned int Height;
	GetDimensions(Width, Height);
	return glm::scale(glm::identity<glm::mat4>(), glm::vec3(Width, Height, 1.0f));
}

UTexture2D* USprite::GetTexture() const
{
	return Texture;
}