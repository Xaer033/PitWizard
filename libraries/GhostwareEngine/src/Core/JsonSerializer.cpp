
#include "JsonSerializer.h"
#include <fstream>
#include "Log.h"

namespace GG
{
	JsonSerializer::JsonSerializer(json & jsonObject, SerializeMode mode): 
		Serializer(mode),
		js(jsonObject)
	{
	}
	
	void JsonSerializer::saveToFile(const std::string & filePath) const
	{
		std::ofstream file(filePath);
		file << js.toStyledString();
		file.close();
	}

	void JsonSerializer::serialize(char & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<char>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %c", info_.name, val_);
		}
	}

	void JsonSerializer::serialize(wchar_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<wchar_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %C", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(int8_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<int8_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}

	void JsonSerializer::serialize(int16_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<int16_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(int32_t & val_, const VarInfo & info_){
		Json::Value value;
		serialize_json<int32_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(int64_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<int64_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(uint8_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<uint8_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(uint16_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<uint16_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(uint32_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<uint32_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(uint64_t & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<uint64_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %d", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(float & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<float>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %f", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(double & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<double>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %f", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(long double & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<double>(this, (double)val_, info_, value); // Truncate it
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %f", info_.name, val_);
		}
	}
	void JsonSerializer::serialize(bool & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<int16_t>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %s", info_.name, val_ ? "true" : "false");
		}
	}

	void JsonSerializer::serialize(std::string & val_, const VarInfo & info_)
	{
		Json::Value value;
		serialize_json<std::string>(this, val_, info_, value);
		if(value == Json::nullValue)
		{
			TRACE_WARNING("Could not serialize: %s with value %s", info_.name, val_);
		}
	}
}
