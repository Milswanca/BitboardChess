#pragma once
#include <glm/glm.hpp>

class USprite;
class UMaterial;

class IRendererImmediate
{
public:
	virtual void DrawSprite(USprite* Sprite, UMaterial* Material, const glm::mat4& Transform) = 0;
};

