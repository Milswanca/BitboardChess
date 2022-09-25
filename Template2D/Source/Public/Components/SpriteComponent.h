#pragma once
#include "Core/Component.h"

class UMesh;
class USprite;
class UShader;
class UMaterial;

class USpriteComponent : public UComponent
{
protected:
	IMPLEMENT_CONSTRUCTOR(USpriteComponent, UComponent);

	virtual void Begin() override;
	virtual void Tick(float dt) override;
	virtual void End() override;

public:
	void SetSprite(USprite* InSprite);
	USprite* GetSprite() const;

	void SetMaterial(UMaterial* InMaterial);
	UMaterial* GetMaterial() const;

private:
	USprite* Sprite;
	UMaterial* Material;
};