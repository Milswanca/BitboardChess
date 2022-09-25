#pragma once
#include "Core/Actor.h"
#include <glm/glm.hpp>
#include "glm/ext/quaternion_float.hpp"

class AActor;

struct FActorSpawnParams
{
	glm::vec3 Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat Rotation = glm::identity<glm::quat>();
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

class UWorld : public UObject
{
	IMPLEMENT_CONSTRUCTOR(UWorld, UObject);

public:
	virtual void Begin() override;
	void TickWorld(float dt);
	virtual UWorld* GetWorld() const override;

	template<typename T>
	T* SpawnActor()
	{
		FActorSpawnParams Params;
		return SpawnActor<T>(Params);
	}

	template<typename T>
	T* SpawnActor(const FActorSpawnParams& Params)
	{
		T* NewActor = GetEngine()->NewObject<T>(this);
		NewActor->SetLocalPosition(Params.Translation);
		NewActor->SetLocalRotation(Params.Rotation);
		NewActor->SetLocalScale(Params.Scale);
		Actors.push_back(NewActor);
		return NewActor;
	}

	void DestroyActor(AActor* ToDestroy)
	{
		Actors.erase(std::remove(Actors.begin(), Actors.end(), ToDestroy), Actors.end());
		GetEngine()->DestroyObject(ToDestroy);
	}

private:
	std::vector<AActor*> Actors;
	UWorld* ConstThis;
};

