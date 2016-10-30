
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
		_height(0),
		IResource()
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
		shutdown();
	}

	void Texture2D::init()
	{
	}

	void Texture2D::shutdown()
	{
		if(_id)
			glDeleteTextures(1, &_id);

		_id = 0;
	}

	IResourceDescriptor * Texture2D::getDescriptor()
	{
		return &_descriptor;
	}

	StringId	Texture2D::getType() const
	{
		return Texture2D::GetResourceType();
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

	bool Texture2D::uploadToGPU(const CIwImage & image)
	{
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

