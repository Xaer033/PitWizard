#pragma once

#include "ShaderLoader.h"

#include <string>
#include <cassert>

#include <GG/Core/Log.h>
#include <GG/Core/FileStream.h>
#include "Shader.h"


namespace GG
{
	std::string ReadStringFromStream(IStream * stream)
	{
		uint32 size = (uint32)stream->getSize();
		char * buffer = (char*)malloc(size);
		stream->read((void*)buffer, sizeof(char), size);

		std::string resultStr(buffer, size);

		free(buffer);
		return resultStr;
	}
	bool ShaderLoader::loadFromFile(IResource * outResource, const std::string & filePath)
	{
		TRACE_ERROR("Loading shader from file not yet supported");
		return false;
	}

	bool ShaderLoader::loadFromMemory(IResource * outResource, uint64 size, const void* data)
	{

		TRACE_ERROR("Loading shader from memory not yet supported");
		return false;
	}

	bool ShaderLoader::loadFromStream(IResource * outResource, IStream * stream)
	{

		TRACE_ERROR("Loading shader from stream not yet supported");
		return false;
	}

	bool ShaderLoader::loadFromDescriptor(
		IResource * outResource,
		const IResourceDescriptor * descriptor)
	{
		ShaderDescriptor * shaderDesc = (ShaderDescriptor*)descriptor;
		assert(shaderDesc != nullptr);

		Shader * shader = (Shader*)outResource;
		
		auto it = shaderDesc->vertexAttributeLayout.cbegin();
		for(; it != shaderDesc->vertexAttributeLayout.cend(); ++it)
		{
			shader->bindAttribute((int)it->first, it->second);
		}

	// Vertex shader
		FileStream vertShaderStream(shaderDesc->vertexShaderSource, OpenMode::OPEN_READ);
		std::string vertShader = ReadStringFromStream(&vertShaderStream);
		vertShaderStream.close();
	
	// Pixel shader
		FileStream pixelShaderStream(shaderDesc->pixelShaderSource, OpenMode::OPEN_READ);
		std::string pixelShader = ReadStringFromStream(&pixelShaderStream);
		pixelShaderStream.close();

		return shader->compile(vertShader, pixelShader);
	}
}