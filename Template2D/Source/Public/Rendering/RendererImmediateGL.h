#pragma once
#include "Core/Object.h"
#include "Rendering/IRendererImmediate.h"

class UMesh;
class UShader;
class UMaterial;

class URendererImmediateGL : public UObject, public IRendererImmediate
{
protected:
	IMPLEMENT_CONSTRUCTOR(URendererImmediateGL, UObject);

	virtual void Begin() override;
	virtual void End() override;

public:
	virtual void DrawSprite(USprite* Sprite, UMaterial* Material, const glm::vec4& Tint, const glm::mat4& Transform) override;

private:
	static UMesh* QuadMesh;
	static UShader* SpriteDefaultShader;
	static UMaterial* SpriteDefaultMaterial;
};

