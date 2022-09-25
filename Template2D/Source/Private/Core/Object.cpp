#include "Core/Object.h"
#include "Core/Engine.h"

UObject::UObject(UObject* InOuter)
{
	Outer = InOuter;
	CurrentWorld = Outer != nullptr ? Outer->GetWorld() : nullptr;
}

UObject::~UObject()
{

}

void UObject::Begin()
{

}

void UObject::End()
{

}

IRendererImmediate* UObject::GetImmediateRenderer() const
{
	return GetEngine()->GetImmediateRenderer();
}

UEngine* UObject::GetEngine() const
{
	return UEngine::Get();
}

UWorld* UObject::GetWorld() const
{
	return CurrentWorld;
}

UObject* UObject::GetOuter() const
{
	return Outer;
}