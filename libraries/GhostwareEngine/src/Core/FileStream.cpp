
#include "FileStream.h"
#include <GG/Core/Log.h>

#include <Physfs/physfs.h>

namespace GG
{
	FileStream::FileStream() : 
		_file(nullptr)
	{

	}

	FileStream::FileStream(const std::string & fileName, const OpenMode & openMode)
	{
		open(fileName, openMode);
	}

	FileStream::~FileStream()
	{
		close();
	}


	bool FileStream::isOpen() const
	{
		return _file != nullptr;
	}

	bool FileStream::open(const std::string & fileName, const OpenMode & openMode)
	{
		if(_file)
			close();

		_file = _openFile(fileName, openMode);
		return _file != nullptr;
	}
	
	int FileStream::close()
	{
		if(!_file)
			return -1;

		return PHYSFS_close(_file);
	}

	uint64 FileStream::getSize() const
	{
		if(!isOpen())
		{
			TRACE_WARNING("Trying to get size from an invalid file stream!");
			return -1;
		}
		return PHYSFS_fileLength(_file);
	}


	int64 FileStream::read(void* buffer, uint32 size, uint32 count)
	{
		if(!isOpen()) 
		{
			TRACE_WARNING("Trying to read from an invalid file stream!");
			return -1; 
		}

		return PHYSFS_read(_file, buffer, size, count); 
	}

	int64 FileStream::write(const void* buffer, uint32 size, uint32 count)
	{
		if(!isOpen())
		{
			TRACE_WARNING("Trying to write to an invalid file stream!");
			return -1;
		}

		return PHYSFS_write(_file, buffer, size, count);
	}

	int64 FileStream::seek(uint64 position)
	{
		if(!isOpen())
		{
			TRACE_WARNING("Trying to seek in an invalid file stream!");
			return -1;
		}

		return PHYSFS_seek(_file, position);
	}

	// --------------------------------------------------------------------------------

	PHYSFS_File * FileStream::_openFile(
		const std::string & fileName, 
		const OpenMode & openMode)	const
	{
		switch(openMode)
		{
		case OpenMode::OPEN_READ:	return PHYSFS_openRead(fileName.c_str());
		case OpenMode::OPEN_WRITE:	return PHYSFS_openWrite(fileName.c_str());
		case OpenMode::OPEN_APPEND:	return PHYSFS_openAppend(fileName.c_str());
		}
		return nullptr;
	}
}