
#include "Texture.h"

#include "IwGL.h"
#include "IwImage.h"

namespace GG
{

	Texture::Texture() : 
		_image(nullptr), 
		_id(0)
	{
	}

	Texture::Texture(CIwImage & image)
	{
		loadFromImage(image);
	}

	Texture::~Texture()
	{
		//we dont own image, so dont free
		_image = nullptr;

		if(_id)
			glDeleteTextures(1, &_id);
	}


	void Texture::loadFromImage(CIwImage & image)
	{
		_image = &image;
	}

	bool Texture::loadFromFile(const std::string & location)
	{
		if(!_image)
			_image = new CIwImage();///YIKES get a resource manager quick!!!

		_image->LoadFromFile(location.c_str());

		return true;
	}

	uint Texture::getWidth() const
	{
		if(_image)
			return _image->GetWidth();

		return 0;
	}

	uint Texture::getHeight() const
	{
		if(_image)
			return _image->GetHeight();

		return 0;
	}

	uint Texture::getId() const
	{
		return _id;
	}


	bool Texture::uploadToGPU()
	{
		if(_image == nullptr)
			return false;

		int internalFormat = (_image->HasAlpha() > 0) ? GL_RGBA : GL_RGB;

		if(_id == 0)
			glGenTextures(1, &_id);

		glBindTexture(GL_TEXTURE_2D, _id);

		//gluBuild2DMipmaps( GL_TEXTURE_2D, 4, texture.getSize().x, texture.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, texture.getPixelsPtr() );
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _image->GetWidth(), _image->GetHeight(), 0, internalFormat, GL_UNSIGNED_BYTE, _image->GetTexels());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}
}

