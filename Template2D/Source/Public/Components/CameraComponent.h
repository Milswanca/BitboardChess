#pragma once
#include "Core/Component.h"

enum class EProjectionTypes
{
	Orthographic,
	Perspective
};

enum class EOrthoFitMode
{
	Stretch,
	Fit,
	FlexibleWidth,
	FlexibleHeight
};

class UCameraComponent : public UComponent
{
protected:
	IMPLEMENT_CONSTRUCTOR(UCameraComponent, UComponent);

	virtual void Begin() override;
	virtual void Tick(float dt) override;
	virtual void End() override;

public:
	void SetAsMain();
	static UCameraComponent* GetMain();

	void SetUseWindowAspect(bool bInUseWindowAspect);
	void SetProjectionType(EProjectionTypes Type);
	void SetFOV(float NewFOV);
	void SetNear(float NewNear);
	void SetFar(float NewFar);
	void SetOrthoSize(float Width, float Height);
	void SetOrthoFitMode(EOrthoFitMode FitMode);
	void DeprojectScreenToWorld(const glm::vec3& Screen, glm::vec3& Location, glm::vec3& Direction);

	bool GetUseWindowAspect() const;
	EProjectionTypes GetProjectionType() const;
	float GetFOV() const;
	float GetNear() const;
	float GetFar() const;

	glm::mat4 GetProjection() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProjectionView() const;

private:
	void RefreshProjectionView();

	bool bUseWindowAspect;
	EProjectionTypes ProjectionType;
	float FOV;
	float Near;
	float Far;
	float Aspect;
	float OrthoHeight;
	float OrthoWidth;
	EOrthoFitMode OrthoFitMode;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 ProjectionView;

	static UCameraComponent* MainCamera;
};

