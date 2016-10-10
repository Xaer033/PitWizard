// Ghostware Games inc. 2016  -Julian Williams
#pragma once


#include <string>
#include "Types.h"


namespace GG
{

	enum class SerializeMode
	{
		SERIALIZE,
		DESERIALIZE
	};

	struct VarInfo
	{
		VarInfo(const std::string & name_) :
			name(name_)
		{}

		virtual ~VarInfo() {}

		std::string name;
	};

	class Serializer
	{
	public:
		Serializer(SerializeMode io_) :
			io(io_)
		{}

		virtual ~Serializer() {}

		virtual void serialize(char & val_, const VarInfo & info_) = 0;
		virtual void serialize(wchar_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(int8_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(int16_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(int32_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(int64_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(uint8_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(uint16_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(uint32_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(uint64_t & val_, const VarInfo & info_) = 0;
		virtual void serialize(float & val_, const VarInfo & info_) = 0;
		virtual void serialize(double & val_, const VarInfo & info_) = 0;
		virtual void serialize(long double & val_, const VarInfo & info_) = 0;
		virtual void serialize(bool & val_, const VarInfo & info_) = 0;
		virtual void serialize(std::string & val_, const VarInfo & info_) = 0;

		SerializeMode io;
	};

	void serialize(Serializer * p, char & val_, const VarInfo & info_);
	void serialize(Serializer * p, wchar_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, int8_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, int16_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, int32_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, int64_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, uint8_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, uint16_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, uint32_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, uint64_t & val_, const VarInfo & info_);
	void serialize(Serializer * p, float & val_, const VarInfo & info_);
	void serialize(Serializer * p, double & val_, const VarInfo & info_);
	void serialize(Serializer * p, long double & val_, const VarInfo & info_);
	void serialize(Serializer * p, bool & val_, const VarInfo & info_);
	void serialize(Serializer * p, std::string & val_, const VarInfo & info_);
}
