#pragma once
#include "Rendering/Texture.h"

class UTexture2D : public UTexture
{
public:
	static UTexture2D* Create(const std::string& File);
	static UTexture2D* Create(unsigned int Width, unsigned int Height, ETextureFormats Format);

protected:
	IMPLEMENT_CONSTRUCTOR(UTexture2D, UTexture);

public:
	virtual void Build() override;
};