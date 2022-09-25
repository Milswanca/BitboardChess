#pragma once
#include "Core/Object.h"
#include <string>
#include <glm/glm.hpp>

class UTexture2D;

class USprite : public UObject
{
public:
	static USprite* Create(const std::string& File);

protected:
	IMPLEMENT_CONSTRUCTOR(USprite, UObject);

	virtual void Begin() override;

public:
	void OverrideDimensions(unsigned int InWidth, unsigned int InHeight);
	void GetDimensions(unsigned int& OutWidth, unsigned int& OutHeight) const;
	glm::mat4 GetScaleMatrix() const;

	UTexture2D* GetTexture() const;

private:
	bool bOverrideDimensions;
	unsigned int OverrideWidth;
	unsigned int OverrideHeight;

	UTexture2D* Texture;
};