/*	VulkanRenderCmdSinkProgram.h
	Copyright 2016 Fabian Herb

	This file is part of Molecular Engine.
*/

#ifndef VULKANRENDERCMDSINKPROGRAM_H
#define VULKANRENDERCMDSINKPROGRAM_H

#include "VulkanRenderCmdSink.h"
#include <unordered_set>

/// Compiled shader program
/** @see Renderer::CreateProgram
	@see Renderer::DestroyProgram */
class VulkanRenderCmdSink::Program : NonCopyable
{
public:
	enum ShaderType
	{
		kFragmentShader,
		kVertexShader,
		kGeometryShader
	};

	struct ShaderSourceItem
	{
		ShaderType type;
		const char* source;
		size_t size;
	};

	/// Compile source code
	/** Legacy interface. */
	void Store(const std::string& vertexShaderSource, const std::string& pixelShaderSource);

	/// Compile source code
	void Store(const ShaderSourceItem items[], size_t count, const char** transformFeedbackOutputs = nullptr, unsigned int tfOutputsCount = 0);

	/// Get set of uniforms and attributes
	/** @see SetUniform
		@see SetAttribute */
	const std::unordered_set<Hash>& GetInputs() const {return mInputs;}

	/// Set uniform values
	/** @param key Hash of the uniform name,
		@param values Pointer to the first (or single) value of the array.
		@param count Elements in the array (or 1 for non-array uniforms). */
	inline void SetUniform(Hash key, const float* values, int count = 1) {}
	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const int* values, int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const unsigned int values[], int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Vector2* values, int count = 1) {}
	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Vector3* values, int count = 1) {}
	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Vector4* values, int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const IntVector2* values, int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const IntVector3* values, int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const IntVector4* values, int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const UIntVector2 values[], int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const UIntVector3 values[], int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const UIntVector4 values[], int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Matrix<3, 3>* values, int count = 1) {}

	/** @copydoc SetUniform */
	inline void SetUniform(Hash key, const Matrix<4, 4>* values, int count = 1) {}

	/// Set texture sampler uniform
	void SetUniform(Hash key, const Texture* const* textures, int count = 1) {}

	/// Set vertex attribute
	void SetAttribute(Hash key, VertexBuffer* buffer, int components, VertexAttributeInfo::Type type, int stride = 0, int offset = 0, bool normalized = true);

	void SetAttribute(Hash key, VertexBuffer* buffer, const VertexAttributeInfo& info)
	{
		SetAttribute(key, buffer, info.components, info.type, info.stride, info.offset, info.normalized);
	}

private:
	std::unordered_set<Hash> mInputs;
};

#endif // VULKANRENDERCMDSINKPROGRAM_H
