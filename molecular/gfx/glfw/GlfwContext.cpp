/*	GlfwContext.cpp
	Copyright 2012-2017 Fabian Herb

	This file is part of Molecular Engine.
*/

#include "GlfwContext.h"
#include <cassert>
#include <GLFW/glfw3.h>

namespace molecular
{

GlfwContext::GlfwContext(GLFWwindow* window) :
	mWindow(window)
{

}

IntVector4 GlfwContext::GetViewport(int eye)
{
	assert(eye == 0);
	int x = 0;
	int y = 0;

    glfwGetFramebufferSize(mWindow, &x, &y);
	return {0, 0, x, y};
}

}
