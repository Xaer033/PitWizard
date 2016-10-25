
#include "MaterialSerializer.h"
#include <algorithm>

#include <GG/Core/json.h>
#include <GG/Core/MathHelper.h>
#include <GG/Core/Log.h>


namespace GG
{
	int MaterialSerializer::VERSION = 1;


	std::string getStringFromBlendMode(const RenderState::BlendMode & blendMode)
	{
		switch(blendMode)
		{
		case RenderState::BlendMode::BM_ADDITIVE:		return "additive";
		case RenderState::BlendMode::BM_ALPHA:			return "alpha";
		case RenderState::BlendMode::BM_MULTIPLY:		return "multiply";
		case RenderState::BlendMode::BM_SCREEN:			return "screen";
		case RenderState::BlendMode::BM_NONE:			return "none";
		}

		TRACE_WARNING("Could not find blend mode: %d", blendMode);
		return "none";
	}
	RenderState::BlendMode getBlendModeFromString(const std::string & blendModeStr)
	{
		std::string key = blendModeStr;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if(key == "additive")		{ return RenderState::BlendMode::BM_ADDITIVE; }
		else if(key == "alpha")		{ return RenderState::BlendMode::BM_ALPHA; }
		else if(key == "multiply")	{ return RenderState::BlendMode::BM_MULTIPLY; }
		else if(key == "screen")	{ return RenderState::BlendMode::BM_SCREEN; }
		else if(key == "none")		{ return RenderState::BlendMode::BM_NONE; }

		TRACE_WARNING("Could not interpret blend mode: %s", key);
		return RenderState::BlendMode::BM_NONE;
	}

	std::string getStringFromCullMode(const RenderState::CullMode & cullMode)
	{
		switch(cullMode)
		{
		case RenderState::CullMode::CULL_FRONT:				return "front";
		case RenderState::CullMode::CULL_BACK:				return "back";
		case RenderState::CullMode::CULL_FRONT_AND_BACK:	return "front_and_back";
		}

		return "none";
	}
	RenderState::CullMode getCullModeFromString(const std::string & cullModeStr)
	{
		std::string key = cullModeStr;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if(key == "front")					{ return RenderState::CullMode::CULL_FRONT;				}
		else if(key == "back")				{ return RenderState::CullMode::CULL_BACK;				}
		else if(key == "front_and_back")	{ return RenderState::CullMode::CULL_FRONT_AND_BACK;	}
		else if(key == "none")				{ return RenderState::CullMode::CULL_NONE;				}

		TRACE_WARNING("Could not interpret cull mode: %s", key);
		return RenderState::CullMode::CULL_NONE;
	}

	std::string getStringFromWindingMode(const RenderState::WindingMode & windingMode)
	{
		switch(windingMode)
		{
		case RenderState::WindingMode::WM_COUNTER_CLOCKWISE:	return "ccw";
		case RenderState::WindingMode::WM_CLOCKWISE:			return "cw";
		}

		return "ccw";
	}
	RenderState::WindingMode getWindingModeFromString(const std::string & windingModeStr)
	{
		std::string key = windingModeStr;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if(key == "ccw") { return RenderState::WindingMode::WM_COUNTER_CLOCKWISE; }
		else if(key == "cw") { return RenderState::WindingMode::WM_CLOCKWISE; }

		TRACE_WARNING("Could not interpret winding mode: %s", key);
		return RenderState::WindingMode::WM_COUNTER_CLOCKWISE;
	}


	bool MaterialSerializer::serialize(const Material & material, std::string & jsonString) const
	{
		json j;

		json renderState = Json::Value( Json::objectValue );
		const RenderStateBlock & block	= material.renderStateBlock;
		renderState["depthTest"]		= block.isDepthTesting;
		renderState["depthRange"]		= ToJson(block.depthRange);
		renderState["cullMode"]			= getStringFromCullMode(block.cullMode);
		renderState["blendMode"]		= getStringFromBlendMode(block.blendMode);
		renderState["windingMode"]		= getStringFromWindingMode(block.windingMode);
		renderState["shaderId"]			= block.shaderId;

		json materialValues = Json::Value(Json::arrayValue);
		
		uint uniformCount = 0;
		for(const auto & pair : material._intUniforms)
		{
			json uniform;
			uniform["name"]		= pair.first;
			uniform["type"]		= "int";
			uniform["value"]	= pair.second;

			materialValues[uniformCount] = uniform;
			++uniformCount;
		}

		for(const auto & pair : material._floatUniforms)
		{
			json uniform;
			uniform["name"]		= pair.first;
			uniform["type"]		= "float";
			uniform["value"]	= pair.second;

			materialValues[uniformCount] = uniform;
			++uniformCount;
		}

		for(const auto & pair : material._vector2Uniforms)
		{
			json uniform;
			uniform["name"]		= pair.first;
			uniform["type"]		= "vector2";
			uniform["value"]	= ToJson(pair.second);

			materialValues[uniformCount] = uniform;
			++uniformCount;
		}

		for(const auto & pair : material._vector3Uniforms)
		{
			json uniform;
			uniform["name"]		= pair.first;
			uniform["type"]		= "vector3";
			uniform["value"]	= ToJson(pair.second);

			materialValues[uniformCount] = uniform;
			++uniformCount;
		}

		for(const auto & pair : material._vector4Uniforms)
		{
			json uniform;
			uniform["name"]		= pair.first;
			uniform["type"]		= "vector4";
			uniform["value"]	= ToJson(pair.second);

			materialValues[uniformCount] = uniform;
			++uniformCount;
		}

		for(const auto & pair : material._textureUniforms)
		{
			json uniform; 
			uniform["name"]		= pair.first;
			uniform["type"]		= "texture";
			uniform["value"]	= pair.second;

			materialValues[uniformCount] = uniform;
			++uniformCount;
		}


		j["materialValues"]		= materialValues;
		j["renderStateBlock"]	= renderState;
		j["version"]			= VERSION;

		jsonString = j.toStyledString();
		return true;
	}

	bool MaterialSerializer::deserialize(Material & material, const std::string & jsonString) const
	{
		json j = JsonFromString(jsonString);

		int version = j["version"].asInt();

		json renderState = j["renderStateBlock"];

		RenderStateBlock & block	= material.renderStateBlock;
		block.isDepthTesting		= renderState["depthTest"].asBool();
		block.depthRange			= Vector2FromJson(renderState["depthRange"]);
		block.cullMode				= getCullModeFromString(renderState["cullMode"].asString());
		block.blendMode				= getBlendModeFromString(renderState["blendMode"].asString());
		block.windingMode			= getWindingModeFromString(renderState["windingMode"].asString());
		block.shaderId				= renderState["shaderId"].asInt();

		json materialValues = j["materialValues"];

		for(uint i = 0; i < materialValues.size(); ++i)
		{
			json uniform = materialValues[i];
			std::string name	= uniform["name"].asString();
			std::string type	= uniform["type"].asString();
			json value			= uniform["value"];

			if(type == "int")
			{
				material._intUniforms[name] = value.asInt();
			}
			else if(type == "float")
			{
				material._floatUniforms[name] = value.asFloat();
			}
			else if(type == "vector2")
			{
				material._vector2Uniforms[name] = Vector2FromJson(value);
			}
			else if(type == "vector3")
			{
				material._vector3Uniforms[name] = Vector3FromJson(value);
			}
			else if(type == "vector4")
			{
				material._vector4Uniforms[name] = Vector4FromJson(value);
			}
			else if(type == "texture")
			{
				material._textureUniforms[name] = value.asUInt();
			}
		}
		

		return true;
	}

}