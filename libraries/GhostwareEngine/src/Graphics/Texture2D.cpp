
#include "Texture2D.h"
#include "s3e.h"
#include "IwGL.h"
#include "IwImage.h"

#include <GG/Core/Log.h>

namespace GG
{

	Texture2D::Texture2D() :
		_id(0),
		_width(0),
		_height(0)
	{
	}

	Texture2D::Texture2D(const json & jsonDescriptor) :
		_id(0),
		_width(0),
		_height(0)
	{
		_descriptor = Texture2DDescriptor::FromJson(jsonDescriptor);
	}

	Texture2D::~Texture2D()
	{
		if(_id)
			glDeleteTextures(1, &_id);

		TRACE_DEBUG("Texture deleted");
	}

	bool Texture2D::loadFromFile(const std::string & location)
	{
		CIwImage image;
		image.LoadFromFile(location.c_str());
		_uploadToGPU(image);
		return true;
	}

	bool Texture2D::loadFromMemory(uint32 size, const void* data)
	{
		CIwImage image;
		s3eFile * f = s3eFileOpenFromMemory((void*)data, size);
		image.ReadFile(f);
		s3eFileClose(f);
		_uploadToGPU(image);
		return true;
	}

	bool Texture2D::loadFromStream(IStream * stream)
	{
		if(stream == nullptr)
		{
			TRACE_WARNING("Stream pointer not valid!");
			return false;
		}


		uint32 size = (uint32)stream->getSize();
		char * buffer = (char*)malloc(size);
		stream->read((void*)buffer, sizeof(char), size);
		loadFromMemory(size, buffer);

		free(buffer);
		return true;
	}

	IResourceDescriptor * Texture2D::getDescriptor()
	{
		return &_descriptor;
	}

	uint Texture2D::getWidth() const
	{
		return _width;
	}

	uint Texture2D::getHeight() const
	{
		return _height;
	}

	uint Texture2D::getId() const
	{
		return _id;
	}


	bool Texture2D::_uploadToGPU(const CIwImage & image)
	{
		/*Texture2DDescriptor * texDesc= (Texture2DDescriptor*)desc;
		if(!texDesc) return false;
*/
		if(_id == 0)
			glGenTextures(1, &_id);

		int internalFormat = (image.HasAlpha() > 0) ? GL_RGBA : GL_RGB;
		_width	= image.GetWidth();
		_height = image.GetHeight();

		glBindTexture(GL_TEXTURE_2D, _id);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, internalFormat, GL_UNSIGNED_BYTE, image.GetTexels());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}
}

