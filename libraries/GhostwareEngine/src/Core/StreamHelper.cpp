#include "StreamHelper.h"
#include <iostream>

namespace GG
{
	namespace Stream
	{
		std::string ReadStringFromStream(IStream * stream)
		{
			uint32 size = (uint32)stream->getSize();
			if(size == 0)
				return "";

			char * buffer = (char*)malloc(size);
			stream->read((void*)buffer, sizeof(char), size);

			std::string resultStr(buffer, size);

			free(buffer);
			return resultStr;
		}
	}
}