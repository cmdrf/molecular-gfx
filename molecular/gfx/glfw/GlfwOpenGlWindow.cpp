/*	GlfwOpenGlWindow.cpp

MIT License

Copyright (c) 2019 Fabian Herb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GlfwOpenGlWindow.h"
#include <molecular/gfx/opengl/OpenGlPrerequisites.h>
#include <stdexcept>

namespace molecular
{
GlfwOpenGlWindow::GlfwOpenGlWindow(const char* title)
{
	if(glfwInit() == GLFW_FALSE)
	{
#ifdef __WIN32
		int error = GetLastError();
		std::ostringstream oss;
		oss << "Error initializing GLFW: " << error;
		throw std::runtime_error(oss.str());
#else
		throw std::runtime_error("Error initializing GLFW");
#endif
	}
	auto monitor = glfwGetPrimaryMonitor();
	auto videoMode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

	//	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

	mWindow = glfwCreateWindow(1024, 768, title, nullptr, nullptr);
	if(!mWindow)
	{
		glfwTerminate();
		throw std::runtime_error("Error opening window");
	}

	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1); // VSync on

	// Clear at least once to work around driver bugs:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GlfwOpenGlWindow::~GlfwOpenGlWindow()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}
}
