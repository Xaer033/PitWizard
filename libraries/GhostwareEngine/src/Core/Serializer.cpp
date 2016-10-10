
#include <GG/Core/ISerializer.h>

#include "Types.h"

namespace GG
{

	void serialize(Serializer * p, char & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, wchar_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, int8_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, int16_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, int32_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, int64_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, uint8_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, uint16_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, uint32_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, uint64_t & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, float & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, double & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, long double & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, bool & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}

	void serialize(Serializer * p, std::string & val_, const VarInfo & info_)
	{
		p->serialize(val_, info_);
	}
}