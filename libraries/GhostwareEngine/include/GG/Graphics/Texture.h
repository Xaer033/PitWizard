#pragma once

#include <string>
#include <GG/Core/Types.h>

class CIwImage;//Get Rid of this soon!!!


namespace GG
{

	class Texture
	{
	public:

		Texture();
		Texture(CIwImage & image);

		~Texture();

		void	loadFromImage(CIwImage & image);
		bool	loadFromFile(const std::string & location);

		bool	uploadToGPU();

		uint	getWidth()  const;
		uint	getHeight() const;

		uint	getId() const;


	private:
		uint	_id;

		CIwImage * _image;
	};



}



