#include "Core/Component.h"
#include "Math/Transform.h"

void UComponent::Begin()
{
	Trans = new FTransform();
	ParentComponent = nullptr;
}

void UComponent::Tick(float dt)
{

}

void UComponent::End()
{
	delete Trans;
	Trans = nullptr;
}

void UComponent::TickComponentTree(float dt)
{
	Tick(dt);

	for (int i = 0; i < Children.size(); ++i)
	{
		Children[i]->TickComponentTree(dt);
	}
}

void UComponent::AttachTo(UComponent* Other)
{
	if (ParentComponent == Other)
		return;

	if (ParentComponent != nullptr)
	{
		ParentComponent->RemoveChild(this);
	}

	FTransform* ParentTrans = Other != nullptr ? Other->Trans : nullptr;
	Trans->SetParent(ParentTrans);

	if (Other)
	{
		Other->AddChild(this);
	}
}

void UComponent::AddChild(UComponent* Child)
{
	if (std::find(Children.begin(), Children.end(), Child) != Children.end())
		return;

	Children.push_back(Child);
}

void UComponent::RemoveChild(UComponent* Child)
{
	Children.erase(std::remove(Children.begin(), Children.end(), Child), Children.end());
}

void UComponent::SetLocalPosition(const glm::vec3& LocalPosition)
{
	Trans->SetLocalPosition(LocalPosition);
}

void UComponent::SetLocalRotation(const glm::quat& LocalRotation)
{
	Trans->SetLocalRotation(LocalRotation);
}

void UComponent::SetLocalScale(const glm::vec3& LocalScale)
{
	Trans->SetLocalScale(LocalScale);
}

void UComponent::TranslateLocal(const glm::vec3& Translation)
{
	Trans->TranslateLocal(Translation);
}

void UComponent::TranslateWorld(const glm::vec3& Translation)
{
	Trans->TranslateWorld(Translation);
}

void UComponent::Rotate(float Degrees, const glm::vec3& Axis)
{
	Trans->Rotate(Degrees, Axis);
}

void UComponent::Rotate(const glm::quat& Quaternion)
{
	Trans->Rotate(Quaternion);
}

void UComponent::Rotate(const glm::vec3& Euler)
{
	Trans->Rotate(Euler);
}

glm::vec3 UComponent::GetLocalPosition() const
{
	return Trans->GetLocalPosition();
}

glm::quat UComponent::GetLocalRotation() const
{
	return Trans->GetLocalRotation();
}

glm::vec3 UComponent::GetLocalScale() const
{
	return Trans->GetLocalScale();
}

glm::vec3 UComponent::GetPosition() const
{
	return Trans->GetPosition();
}

glm::quat UComponent::GetRotation() const
{
	return Trans->GetRotation();
}

glm::vec3 UComponent::GetForward() const
{
	return Trans->GetForward();
}

glm::vec3 UComponent::GetRight() const
{
	return Trans->GetRight();
}

glm::vec3 UComponent::GetUp() const
{
	return Trans->GetUp();
}

void UComponent::GetLocalToWorld(glm::mat4& OutLocalToWorld) const
{
	Trans->GetLocalToWorld(OutLocalToWorld);
}

void UComponent::GetWorldToLocal(glm::mat4& OutWorldToLocal) const
{
	Trans->GetWorldToLocal(OutWorldToLocal);
}

UComponent* UComponent::GetParent() const
{
	return ParentComponent;
}

UComponent* UComponent::GetChild(int Index) const
{
	return Children[Index];
}

int UComponent::GetNumChildren() const
{
	return Children.size();
}