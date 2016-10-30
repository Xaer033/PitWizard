// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>

#include <GG/Resources/IResourceDescriptor.h>
#include <GG/Core/Log.h>

#include "VertexLayout.h"

namespace GG
{
	typedef std::unordered_map<uint, std::string>	VertexAttributeLayout;


	struct ShaderDescriptor : public IResourceDescriptor
	{
		static const ShaderDescriptor FromJson(const json & j)
		{
			ShaderDescriptor desc;
			desc.resourceId				= STRING_ID(j.get("id", json("null")).asString());
			desc.type					= STRING_ID(j.get("type", json("null")).asString());
			desc.source					= j.get("source", json("null")).asString();
			desc.vertexShaderSource		= j.get("vertexSource",	json("null")).asString();
			desc.pixelShaderSource		= j.get("pixelSource",	json("null")).asString();
			
			json attributeLayout		= j.get("vertexAttributeLayout", Json::objectValue);
			for(auto it = attributeLayout.begin(); it != attributeLayout.end(); ++it)
			{
				uint attribKey = _GetVertexAttributeFromString(it.key().asString());
				std::string attribName = it->asString();
				desc.vertexAttributeLayout[attribKey] = attribName;
			}

			return desc;
		}

		ShaderDescriptor() :
			IResourceDescriptor()
		{
		}

		std::string				vertexShaderSource;
		std::string				pixelShaderSource;

		VertexAttributeLayout	vertexAttributeLayout;

	private:

		static VertexTags _GetVertexAttributeFromString(const std::string & attribStr)
		{
			std::string key = attribStr;
			std::transform(key.begin(), key.end(), key.begin(), ::tolower);

			if(key == "position")		{ return VertexTags::Position; }
			else if(key == "uv0")		{ return VertexTags::Uv0; }
			else if(key == "normal")	{ return VertexTags::Normal; }
			else if(key == "tangent")	{ return VertexTags::Tangent; }
			else if(key == "bitangent") { return VertexTags::Bitangent; }
			else if(key == "color")		{ return VertexTags::Color; }
		
			TRACE_WARNING("Vertex Attribute Key not supported: %s", key);
			return (VertexTags)0;
		}
	};
}
