#include "Core/Actor.h"
#include "Core/Engine.h"
#include "Core/World.h"

void AActor::Begin()
{
	RootComponent = AddComponent<UComponent>();
}

void AActor::Tick(float dt)
{
	RootComponent->TickComponentTree(dt);
}

void AActor::End()
{
	DestroyComponentTree(RootComponent);
}

void AActor::SetLocalPosition(const glm::vec3& LocalPosition)
{
	RootComponent->SetLocalPosition(LocalPosition);
}

void AActor::SetLocalRotation(const glm::quat& LocalRotation)
{
	RootComponent->SetLocalRotation(LocalRotation);
}

void AActor::SetLocalScale(const glm::vec3& LocalScale)
{
	RootComponent->SetLocalScale(LocalScale);
}

void AActor::TranslateLocal(const glm::vec3& Translation)
{
	RootComponent->TranslateLocal(Translation);
}

void AActor::TranslateWorld(const glm::vec3& Translation)
{
	RootComponent->TranslateWorld(Translation);
}

void AActor::Rotate(float Degrees, const glm::vec3& Axis)
{
	RootComponent->Rotate(Degrees, Axis);
}

void AActor::Rotate(const glm::quat& Quaternion)
{
	RootComponent->Rotate(Quaternion);
}

void AActor::Rotate(const glm::vec3& Euler)
{
	RootComponent->Rotate(Euler);
}

glm::vec3 AActor::GetLocalPosition() const
{
	return RootComponent->GetLocalPosition();
}

glm::quat AActor::GetLocalRotation() const
{
	return RootComponent->GetLocalRotation();
}

glm::vec3 AActor::GetLocalScale() const
{
	return RootComponent->GetLocalScale();
}

glm::vec3 AActor::GetPosition() const
{
	return RootComponent->GetPosition();
}

glm::quat AActor::GetRotation() const
{
	return RootComponent->GetRotation();
}

glm::vec3 AActor::GetForward() const
{
	return RootComponent->GetForward();
}

glm::vec3 AActor::GetRight() const
{
	return RootComponent->GetRight();
}

glm::vec3 AActor::GetUp() const
{
	return RootComponent->GetUp();
}

void AActor::GetLocalToWorld(glm::mat4& OutLocalToWorld) const
{
	RootComponent->GetLocalToWorld(OutLocalToWorld);
}

void AActor::GetWorldToLocal(glm::mat4& OutWorldToLocal) const
{
	RootComponent->GetWorldToLocal(OutWorldToLocal);
}

void AActor::DestroyActor()
{
	GetWorld()->DestroyActor(this);
}

void AActor::DestroyComponent(UComponent* Comp, bool bRemoveChildren)
{
	if (Comp == RootComponent)
		return;

	if (bRemoveChildren)
	{
		DestroyComponentTree(Comp);
	}
	else
	{
		UComponent* Parent = Comp->GetParent();
		int NumChildren = Comp->GetNumChildren();

		for (int i = 0; i < NumChildren; ++i)
		{
			UComponent* Child = Comp->GetChild(i);
			Child->AttachTo(Parent);
		}

		UEngine::Get()->DestroyObject(Comp);
	}
}

void AActor::DestroyComponentTree(UComponent* Comp)
{
	int NumChildren = Comp->GetNumChildren();

	for (int i = 0; i < NumChildren; ++i)
	{
		DestroyComponentTree(Comp->GetChild(i));
	}

	UEngine::Get()->DestroyObject(Comp);
}

UComponent* AActor::GetRootComponent() const
{
	return RootComponent;
}