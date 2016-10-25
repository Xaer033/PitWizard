// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <GG/Core/Types.h>
#include <GG/Core/StringId.h>
#include <GG/Core/Json.h>

#include <GG/Resources/IResource.h>
#include <GG/Resources/IResourceDescriptor.h>

class CIwImage;//Get Rid of this soon!!!


namespace GG
{

	class Texture2D : public IResource
	{
	public:
		static StringId GetResourceType()
		{
			static StringId id = STRING_ID("Texture2D");
			return id;
		}

	public:

		Texture2D();
		Texture2D(const json & jsonDescriptor);

		virtual ~Texture2D();

		virtual bool	loadFromFile(const std::string & location)		final;
		virtual bool	loadFromMemory(uint32 size, const void* data)	final;
		virtual bool	loadFromStream(IStream * stream)				final;
		virtual IResourceDescriptor* getDescriptor()					final;


		uint	getWidth()  const;
		uint	getHeight() const;

		uint	getId()		const;


	private:
		uint	_id;

		uint	_width;
		uint	_height;

		bool	_uploadToGPU(const CIwImage & image);

		Texture2DDescriptor _descriptor;
	};



}



