#pragma once

class UEngine;
class UWorld;
class IRendererImmediate;

#define IMPLEMENT_CONSTRUCTOR(className, baseClass) \
    className() = delete; \
	className(UObject* InOuter) : baseClass(InOuter) {} \
    virtual ~className() {} \
	friend class UEngine;

class UObject
{
protected:
	UObject() = delete;
	UObject(UObject* InOuter);
	~UObject();

	virtual void Begin();
	virtual void End();

	UEngine* GetEngine() const;
	IRendererImmediate* GetImmediateRenderer() const;
	virtual UWorld* GetWorld() const;
	virtual UObject* GetOuter() const;

private:
	UObject* Outer;
	UWorld* CurrentWorld;
	friend class UEngine;
};