cmake_minimum_required(VERSION 3.0)
project(runner-glfw)

option(MOLECULAR_BUILD_RUNNER_GLFW "Build GLFW-based Runner Application" Off)

if(MOLECULAR_BUILD_RUNNER_GLFW)
	add_executable(molecularrunner-glfw
		GlfwInputModule.cpp
		GlfwInputModule.h
		GlfwContext.cpp
		GlfwContext.h
		GlfwGraphicsModule.cpp
		GlfwGraphicsModule.h
		GlfwRenderManager.cpp
		GlfwRenderManager.h
		GlfwRoot.cpp
		GlfwRoot.h
		RunnerGlfwMain.cpp
	)
    target_link_libraries(molecularrunner-glfw
		glfw
		molecularengine
		molecularengine-openal
	)
    if(MOLECULAR_ENABLE_VULKAN)
		target_sources(molecularrunner-glfw PRIVATE GlfwVulkanWindow.cpp GlfwVulkanWindow.h)
		target_link_libraries(molecularrunner-glfw Vulkan::Vulkan)
	else()
		target_sources(molecularrunner-glfw PRIVATE GlfwOpenGlWindow.cpp GlfwOpenGlWindow.h)
		target_link_libraries(molecularrunner-glfw opengl)
	endif()

    if(WIN32)
        install(TARGETS molecularrunner-glfw DESTINATION .)
    else()
	    install(TARGETS molecularrunner-glfw DESTINATION bin)
    endif()
endif(MOLECULAR_BUILD_RUNNER_GLFW)
