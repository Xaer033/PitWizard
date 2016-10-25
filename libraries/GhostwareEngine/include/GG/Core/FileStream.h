#pragma once
#include <string>
#include "IStream.h"

struct PHYSFS_File;

namespace GG
{
	enum OpenMode
	{
		OPEN_READ,
		OPEN_WRITE,
		OPEN_APPEND
	};

	class FileStream : public IStream
	{
	public:
		FileStream();
		FileStream(const std::string & fileName, const OpenMode & openMode);
		virtual		~FileStream();

		bool			isOpen() const;
		bool			open(const std::string & fileName, const OpenMode & openMode);
		int				close();

	public:
		virtual int64	read(void* buffer, uint32 size, uint32 count);
		virtual int64	write(const void* buffer, uint32 size, uint32 count);
		virtual int64	seek(uint64 position);
		virtual uint64	getSize() const;

	private:
		PHYSFS_File *	_openFile(const std::string & fileName, const OpenMode & openMode) const;
	private:
		PHYSFS_File *	_file;
	};
}