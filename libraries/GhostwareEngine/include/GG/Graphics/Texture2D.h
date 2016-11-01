// Ghostware Games inc. 2016  -Julian Williams
#pragma once

#include <string>
#include <GG/Core/Types.h>
#include <GG/Core/StringId.h>
#include <GG/Core/Json.h>

#include <GG/Resources/IResource.h>

#include "Texture2DDescriptor.h"

class CIwImage;//Get Rid of this soon!!!


namespace GG
{
	enum TextureWrapMode
	{
		WM_REPEAT	= GL_REPEAT,
		WM_CLAMP	= GL_CLAMP_TO_EDGE,
		WM_MIRRORED_REPEAT = GL_MIRRORED_REPEAT
	};

	enum TextureFilterMode
	{
		FM_NEAREST	= GL_NEAREST,
		FM_LINEAR	= GL_LINEAR,
		FM_MIPMAP_NEAREST	= GL_NEAREST_MIPMAP_NEAREST,
		FM_MIPMAP_LINEAR	= GL_LINEAR_MIPMAP_LINEAR
	};


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

		virtual void	init() final;
		virtual void	shutdown() final;
		virtual IResourceDescriptor*	getDescriptor()	final;
		virtual StringId	getType() const				final;


		uint	getWidth()  const;
		uint	getHeight() const;

		uint	getId()		const;

		bool	uploadToGPU(const CIwImage & image);

	private:
		uint	_id;

		uint	_width;
		uint	_height;


		Texture2DDescriptor _descriptor;
	};



}



