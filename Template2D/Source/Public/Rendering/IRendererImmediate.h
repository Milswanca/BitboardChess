#pragma once
#include <glm/glm.hpp>

class USprite;
class UMaterial;

class IRendererImmediate
{
public:
	void DrawSprite(USprite* Sprite, UMaterial* Material, const glm::mat4& Transform)
	{
		DrawSprite(Sprite, Material, glm::vec4(1.0f), Transform);
	}

	virtual void DrawSprite(USprite* Sprite, UMaterial* Material, const glm::vec4& Tint, const glm::mat4& Transform) = 0;
};

