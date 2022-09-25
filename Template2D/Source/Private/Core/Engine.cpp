#include "Core/Engine.h"
#include "Core/World.h"
#include "Rendering/RendererImmediateGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

unsigned int UEngine::WindowWidth = 0;
unsigned int UEngine::WindowHeight = 0;
UEngine* UEngine::Inst = nullptr;

void framebuffer_size_callback(GLFWwindow* Window, int Width, int Height)
{
	UEngine::WindowWidth = Width;
	UEngine::WindowHeight = Height;
	glViewport(0, 0, Width, Height);
}

void UEngine::Init()
{
	Inst = this;

	MainWindow = nullptr;
	CurrentWorld = nullptr;
	RendererImmediate = nullptr;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void UEngine::InitWorld()
{
	CurrentWorld = NewObject<UWorld>(nullptr);
	RendererImmediate = NewObject<URendererImmediateGL>(nullptr);
}

void UEngine::Shutdown()
{
	glfwTerminate();

	for (int i = Objects.size() - 1; i >= 0; --i)
	{
		delete Objects[i];
	}

	Objects.clear();
}

UEngine* UEngine::Get()
{
	return Inst;
}

IRendererImmediate* UEngine::GetImmediateRenderer() const
{
	return RendererImmediate;
}

UWorld* UEngine::GetWorld() const
{
	return CurrentWorld;
}

GLFWwindow* UEngine::GetMainWindow() const
{
	return MainWindow;
}

int UEngine::CreateMainWindow(int Width, int Height, const std::string& Title)
{
	if (MainWindow != nullptr)
		return -1;

	MainWindow = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
	if (MainWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(MainWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	WindowWidth = Width;
	WindowHeight = Height;
	glViewport(0, 0, Width, Height);
	glfwSetFramebufferSizeCallback(MainWindow, framebuffer_size_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

int UEngine::Run()
{
	while (MainWindow != nullptr && !glfwWindowShouldClose(MainWindow))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		CurrentWorld->TickWorld(0.0f);

		glfwSwapBuffers(MainWindow);
		glfwPollEvents();
	}

	return 0;
}