#pragma once
#include "Core/Object.h"
#include <vector>

#include "glm/glm.hpp"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

struct FTransform;

class UComponent : public UObject
{
protected:
	IMPLEMENT_CONSTRUCTOR(UComponent, UObject);

protected:
	virtual void Begin() override;
	virtual void Tick(float dt);
	virtual void End() override;

public:
	void AttachTo(UComponent* Other);

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

	UComponent* GetParent() const;
	UComponent* GetChild(int Index) const;
	int GetNumChildren() const;

private:
	void TickComponentTree(float dt);

	void AddChild(UComponent* Child);
	void RemoveChild(UComponent* Child);

	std::vector<UComponent*> Children;

	UComponent* ParentComponent;
	FTransform* Trans;

	friend class AActor;
};