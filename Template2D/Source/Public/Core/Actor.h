#pragma once
#include "Core/Object.h"
#include "Core/Component.h"
#include "Core/Engine.h"
#include <vector>

#include "glm/ext/quaternion_float.hpp"

class UComponent;

class AActor : public UObject
{
protected:
	IMPLEMENT_CONSTRUCTOR(AActor, UObject);

protected:
	virtual void Begin() override;
	virtual void Tick(float dt);
	virtual void End() override;

public:
	void SetLocalPosition(const glm::vec3& LocalPosition);
	void SetLocalRotation(const glm::quat& LocalRotation);
	void SetLocalScale(const glm::vec3& LocalScale);

	void TranslateLocal(const glm::vec3& Translation);
	void TranslateWorld(const glm::vec3& Translation);

	void Rotate(float Degrees, const glm::vec3& Axis);
	void Rotate(const glm::quat& Quaternion);
	void Rotate(const glm::vec3& Euler);

	glm::vec3 GetLocalPosition() const;
	glm::quat GetLocalRotation() const;
	glm::vec3 GetLocalScale() const;

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

	void GetLocalToWorld(glm::mat4& OutLocalToWorld) const;
	void GetWorldToLocal(glm::mat4& OutWorldToLocal) const;

	template<typename T>
	T* AddComponent()
	{
		T* NewComp = GetEngine()->NewObject<T>(this);
		Components.push_back(NewComp);

		return NewComp;
	}

	void DestroyComponent(UComponent* Comp, bool bRemoveChildren);
	void DestroyActor();

	UComponent* GetRootComponent() const;

private:
	void DestroyComponentTree(UComponent* Comp);

	UComponent* RootComponent;
	std::vector<UComponent*> Components;

	friend class UWorld;
};