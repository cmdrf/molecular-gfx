configure_file(molecular/Config.h.in molecular/Config.h)

add_library(molecular-gfx
	molecular/gfx/AssetManager.h
	molecular/gfx/CharacterAnimation.cpp
	molecular/gfx/CharacterAnimation.h
	molecular/gfx/DefaultProgramData.cpp
	molecular/gfx/DefaultProgramData.h
	molecular/gfx/DrawText.cpp
	molecular/gfx/DrawText.h
	molecular/gfx/GfxUtils.h
	molecular/gfx/MeshBoundsCollectionFile.h
	molecular/gfx/MeshManager.h
	molecular/gfx/Material.cpp
	molecular/gfx/Material.h
	molecular/gfx/MaterialManager.cpp
	molecular/gfx/MaterialManager.h
	molecular/gfx/MeshDataSource.h
	molecular/gfx/MeshLoader.cpp
	molecular/gfx/MeshLoader.h
	molecular/gfx/MeshLocator.h
	molecular/gfx/NmbMeshDataSource.cpp
	molecular/gfx/NmbMeshDataSource.h
	molecular/gfx/Picking.h
	molecular/gfx/ProgramProvider.cpp
	molecular/gfx/ProgramProvider.h
	molecular/gfx/RenderCmdSink.h
	molecular/gfx/RenderContext.h
	molecular/gfx/RenderFunction.h
	molecular/gfx/RenderManager.h
	molecular/gfx/Scoping.h
	molecular/gfx/SkinProgramData.cpp
	molecular/gfx/SkinProgramData.h
	molecular/gfx/TetrahedronInterpolation.cpp
	molecular/gfx/TetrahedronInterpolation.h
	molecular/gfx/TetrahedronSpaceFile.h
	molecular/gfx/TextureManager.h
	molecular/gfx/TextureUtils.cpp
	molecular/gfx/TextureUtils.h
	molecular/gfx/TextureSplattingData.cpp
	molecular/gfx/TextureSplattingData.h
	molecular/gfx/Uniform.cpp
	molecular/gfx/Uniform.h

	molecular/gfx/glfw/GlfwContext.cpp
	molecular/gfx/glfw/GlfwContext.h
	molecular/gfx/glfw/GlfwFileLoader.h
	molecular/gfx/glfw/GlfwOpenGlWindow.cpp
	molecular/gfx/glfw/GlfwOpenGlWindow.h
	molecular/gfx/glfw/GlfwPrerequisites.h
	molecular/gfx/glfw/GlfwRenderManager.cpp
	molecular/gfx/glfw/GlfwRenderManager.h
#	molecular/gfx/glfw/GlfwVulkanWindow.cpp
#	molecular/gfx/glfw/GlfwVulkanWindow.h
    molecular/gfx/glfw/GlfwWindow.h

    molecular/gfx/functions/ApplyTextures.cpp
	molecular/gfx/functions/ApplyTextures.h
	molecular/gfx/functions/CpuParticleSystemImpl.h
	molecular/gfx/functions/CpuParticleSystem.h
	molecular/gfx/functions/DrawDebugMesh.h
	molecular/gfx/functions/DrawingFunction.h
	molecular/gfx/functions/DrawMesh.h
	molecular/gfx/functions/DrawMeshData.cpp
	molecular/gfx/functions/DrawMeshData.h
	molecular/gfx/functions/DrawSkyDome.h
	molecular/gfx/functions/DrawTerrain.cpp
	molecular/gfx/functions/DrawTerrain.h
	molecular/gfx/functions/DrawTestSkeleton.cpp
	molecular/gfx/functions/DrawTestSkeleton.h
	molecular/gfx/functions/FlatScene.cpp
	molecular/gfx/functions/FlatScene.h
	molecular/gfx/functions/FullscreenEffect.cpp
	molecular/gfx/functions/FullscreenEffect.h
	molecular/gfx/functions/HumanSkin.cpp
	molecular/gfx/functions/HumanSkin.h
	molecular/gfx/functions/ParticleSystem.h
	molecular/gfx/functions/ParticleSystem.cpp
	molecular/gfx/functions/PortalZoneScene.h
	molecular/gfx/functions/PortalZoneSceneImpl.h
	molecular/gfx/functions/RequestOutput.cpp
	molecular/gfx/functions/RequestOutput.h
	#molecular/gfx/functions/RTreeScene.cpp
	#molecular/gfx/functions/RTreeScene.h
	molecular/gfx/functions/SceneSetup.cpp
	molecular/gfx/functions/SceneSetup.h
	molecular/gfx/functions/SetShCoefficients.cpp
	molecular/gfx/functions/SetShCoefficients.h
	molecular/gfx/functions/SetUniforms.cpp
	molecular/gfx/functions/SetUniforms.h
	molecular/gfx/functions/ShadowMapping.cpp
	molecular/gfx/functions/ShadowMapping.h
	molecular/gfx/functions/Skeleton.h
	molecular/gfx/functions/Skeleton.cpp
	molecular/gfx/functions/TextureSplatting.cpp
	molecular/gfx/functions/TextureSplatting.h
	molecular/gfx/functions/Transform.cpp
	molecular/gfx/functions/Transform.h
	molecular/gfx/functions/ViewSetup.cpp
	molecular/gfx/functions/ViewSetup.h
	molecular/gfx/functions/Visibility.cpp
	molecular/gfx/functions/Visibility.h

	molecular/gfx/opengl/Egl.cpp
	molecular/gfx/opengl/Egl.h
	molecular/gfx/opengl/Glx.cpp
	molecular/gfx/opengl/Glx.h
	molecular/gfx/opengl/GlCommandSinkProgram.cpp
	molecular/gfx/opengl/GlCommandSinkProgram.h
	molecular/gfx/opengl/GlFunctionsGl10Native.h
	molecular/gfx/opengl/GlFunctionsGles2Native.h
	molecular/gfx/opengl/GlCommandSink.cpp
	molecular/gfx/opengl/GlFunctionsGles2ProcAddresses.h
	molecular/gfx/opengl/GlFunctionsProcAddresses.h
	molecular/gfx/opengl/OpenGlPrerequisites.h
	molecular/gfx/opengl/GlFunctions.h
	molecular/gfx/opengl/GlConstants.h
	molecular/gfx/opengl/GlConstantString.cpp
	molecular/gfx/opengl/GlConstantString.h
	molecular/gfx/opengl/PixelFormatConversion.cpp
	molecular/gfx/opengl/PixelFormatConversion.h
	molecular/gfx/opengl/GlFunctionsNative.h
	molecular/gfx/opengl/GlCommandSink.h

#	molecular/gfx/vulkan/VulkanInfrastructure.cpp
#	molecular/gfx/vulkan/VulkanInfrastructure.h
#	molecular/gfx/vulkan/VulkanRenderCmdSink.cpp
#	molecular/gfx/vulkan/VulkanRenderCmdSink.h
#	molecular/gfx/vulkan/VulkanRenderCmdSinkProgram.cpp
#	molecular/gfx/vulkan/VulkanRenderCmdSinkProgram.h

	molecular/util/AtomicCounter.h
	molecular/util/Blob.h
	molecular/util/BlobFunctionTask.h
	molecular/util/Box.cpp
	molecular/util/Box.h
	molecular/util/DdsFile.cpp
	molecular/util/DdsFile.h
	molecular/util/DummyFileLoader.h
	molecular/util/DynamicScoping.h
	molecular/util/FileServer.h
	molecular/util/FileTypeIdentification.cpp
	molecular/util/FileTypeIdentification.h
	molecular/util/FontAtlasDescriptionFile.h
	molecular/util/Frustum.cpp
	molecular/util/Frustum.h
	molecular/util/HostStream.h
	molecular/util/IniFile.cpp
	molecular/util/IniFile.h
	molecular/util/IteratorAdapters.h
	molecular/util/KtxFile.cpp
	molecular/util/KtxFile.h
	molecular/util/LittleEndianStream.h
	molecular/util/Logging.h
	molecular/util/MemoryStreamStorage.cpp
	molecular/util/MemoryStreamStorage.h
	molecular/util/MtlFile.cpp
	molecular/util/MtlFile.h
	molecular/util/NmbFile.h
	molecular/util/NonCopyable.h
	molecular/util/PixelFormat.cpp
	molecular/util/PixelFormat.h
	molecular/util/Plane.h
	molecular/util/PointerContainer.h
	molecular/util/RTree.cpp
	molecular/util/RTree.h
	molecular/util/StdTaskQueue.cpp
	molecular/util/StdTaskQueue.h
	molecular/util/StdThread.h
	molecular/util/Stream.cpp
	molecular/util/Stream.h
	molecular/util/StreamBase.cpp
	molecular/util/StreamBase.h
	molecular/util/StreamStorage.cpp
	molecular/util/StreamStorage.h
	molecular/util/StringStore.cpp
	molecular/util/StringStore.h
	molecular/util/Task.h
	molecular/util/TaskDispatcher.h
	molecular/util/TextStream.h
	molecular/util/TgaFile.h
)
add_library(molecular::gfx ALIAS molecular-gfx)

if(APPLE)
	target_compile_definitions(molecular-gfx PUBLIC GL_SILENCE_DEPRECATION)
	target_sources(molecular-gfx PRIVATE
		molecular/util/GcdAsyncFileLoader.h
		molecular/util/GcdTaskDispatcher.cpp
		molecular/util/GcdTaskDispatcher.h
	)
endif()

target_include_directories(molecular-gfx PUBLIC
	.
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(molecular-gfx PUBLIC
	molecular::util
	molecular::programgenerator
	molecular::meshfile
	Eigen3::Eigen
	OpenGL::GL
	glfw
	Threads::Threads
)

add_subdirectory(examples)
if(BUILD_TESTING)
	add_subdirectory(tests)
endif()
