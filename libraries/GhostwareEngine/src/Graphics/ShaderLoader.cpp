#pragma once

#include "ShaderLoader.h"

#include <string>
#include <cassert>

#include <GG/Core/Log.h>
#include <GG/Core/FileStream.h>
#include "Shader.h"
#include <GG/Core/StreamHelper.h>

namespace GG
{
	bool ShaderLoader::loadDefault(IResource * outResource)
	{
		Shader * shader = (Shader*)outResource;
		static const char * vertexShader =
			"uniform mat4 inMVP;\n"
			"attribute vec4 inVert;\n"
			"varying vec3 modelVert;\n"
			"void main() {\n"
			"   gl_Position = inMVP * inVert;\n"
			"   modelVert = inVert.xyz;\n"
			"}\n";
		static const char * pixelShader =
			"precision mediump float;\n"
			"varying vec3 modelVert;\n"
			"void main(){\n"
			"   gl_FragColor = vec4(abs(cos(modelVert.x)), abs(modelVert.y), abs(sin(modelVert.z)), 1);  \n"
			"}\n";

		shader->bindAttribute(VertexTags::Position, "inVert");
		return shader->compile(vertexShader, pixelShader);
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
		std::string vertShader = Stream::ReadStringFromStream(&vertShaderStream);
		vertShaderStream.close();
	
	// Pixel shader
		FileStream pixelShaderStream(shaderDesc->pixelShaderSource, OpenMode::OPEN_READ);
		std::string pixelShader = Stream::ReadStringFromStream(&pixelShaderStream);
		pixelShaderStream.close();

		return shader->compile(vertShader, pixelShader);
	}
}