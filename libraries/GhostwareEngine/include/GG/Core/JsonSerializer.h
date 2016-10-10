#pragma once

#include "ISerializer.h"
#include "Json.h"
#include "Types.h"

namespace GG
{
	class JsonSerializer : public Serializer
	{
	public:
		JsonSerializer(json & jsonObject, SerializeMode mode);
		json & js;

		void saveToFile(const std::string & filePath) const;

		virtual ~JsonSerializer() {}

		virtual void serialize(char & val_, const VarInfo & info_);
		virtual void serialize(wchar_t & val_, const VarInfo & info_);
		virtual void serialize(int8_t & val_, const VarInfo & info_);
		virtual void serialize(int16_t & val_, const VarInfo & info_);
		virtual void serialize(int32_t & val_, const VarInfo & info_);
		virtual void serialize(int64_t & val_, const VarInfo & info_);
		virtual void serialize(uint8_t & val_, const VarInfo & info_);
		virtual void serialize(uint16_t & val_, const VarInfo & info_);
		virtual void serialize(uint32_t & val_, const VarInfo & info_);
		virtual void serialize(uint64_t & val_, const VarInfo & info_);
		virtual void serialize(float & val_, const VarInfo & info_);
		virtual void serialize(double & val_, const VarInfo & info_);
		virtual void serialize(long double & val_, const VarInfo & info_);
		virtual void serialize(bool & val_, const VarInfo & info_);
		virtual void serialize(std::string & val_, const VarInfo & info_);
	};


	template<class T>
	void serialize_json(JsonSerializer * p, T val, const VarInfo & info, Json::Value & value)
	{
		if(p->io == SerializeMode::SERIALIZE)
		{
			p->js[info.name] = val;
		}
		else
		{
			value = p->js[info.name];
		}
	}
}