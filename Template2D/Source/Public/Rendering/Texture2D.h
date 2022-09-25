#pragma once
#include "Rendering/Texture.h"

class UTexture2D : public UTexture
{
public:
	static UTexture2D* Create(const std::string& File);

protected:
	IMPLEMENT_CONSTRUCTOR(UTexture2D, UTexture);

public:
	virtual void Build() override;
};