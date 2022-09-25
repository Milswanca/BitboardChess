#pragma once
#include <string>
#include <vector>
#include "Core/Object.h"

struct GLFWwindow;
class UObject;
class UWorld;
class IRendererImmediate;

#define IMPLEMENT_MAIN(screenWidth, screenHeight, screenTitle, applicationClass) \
	int main() \
	{ \
		UEngine* Eng = new UEngine(nullptr); \
		Eng->Init(); \
		Eng->CreateMainWindow(screenWidth, screenHeight, screenTitle); \
		Eng->InitWorld(); \
		Eng->GetWorld()->SpawnActor<applicationClass>(); \
		Eng->Run(); \
		Eng->Shutdown(); \
		delete Eng; \
		return 0; \
	} \

class UEngine : public UObject
{
public:
	IMPLEMENT_CONSTRUCTOR(UEngine, UObject);

public:
	static unsigned int WindowWidth;
	static unsigned int WindowHeight;
	static UEngine* Get();


	virtual void Init();
	virtual void InitWorld();
	int CreateMainWindow(int Width, int Height, const std::string& Title);
	int Run();
	virtual void Shutdown();

	virtual UWorld* GetWorld() const;
	IRendererImmediate* GetImmediateRenderer() const;
	GLFWwindow* GetMainWindow() const;

	template<typename T>
	T* NewObject(UObject* Outer = nullptr)
	{
		Outer = Outer == nullptr ? Inst : Outer;

		T* NewObj = new T(Outer);
		NewObj->Begin();
		Objects.push_back(NewObj);
		return NewObj;
	}

	void DestroyObject(UObject* Obj)
	{
		if (Obj == nullptr)
			return;
	
		Obj->End();
		Objects.erase(std::remove(Objects.begin(), Objects.end(), Obj), Objects.end());
		delete Obj;
	}

private:
	std::vector<UObject*> Objects;

	IRendererImmediate* RendererImmediate;
	UWorld* CurrentWorld;
	GLFWwindow* MainWindow;
	static UEngine* Inst;
};

