module;

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <iostream>

export module window;

static bool s_showStats = false;

void glfw_errorCallback(int error, const char* description)
{
	std::cout << "GLFW error " << error << " description" << std::endl;
}

void glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	//if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
	//	s_showStats = !s_showStats;
}

export class Window {
private:
	GLFWwindow* window;

public:
	Window() {}
	~Window() 
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	bool init()
	{
		glfwSetErrorCallback(glfw_errorCallback);
		if (!glfwInit())
			return false;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(1024, 768, "helloworld", nullptr, nullptr);
		if (!window)
			return false;
		glfwSetKeyCallback(window, glfw_keyCallback);
		return true;

	}
	HWND getNativeWindow()
	{
		return glfwGetWin32Window(window);
	}
	void getWindowSize(int &width, int &height)
	{
		
		glfwGetWindowSize(window, &width, &height);
	}
	bool shouldClose()
	{
		return glfwWindowShouldClose(window);
	}
	void pollEvents()
	{
		glfwPollEvents();
	}

	

};