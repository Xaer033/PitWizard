
#include "FileStream.h"
#include <GG/Core/Log.h>
#include <GG/Core/FileSystem.h>

namespace GG
{
	FileStream::FileStream() : 
		_file(nullptr)
	{
	}

	FileStream::FileStream(const std::string & fileName, const OpenMode & openMode) :
		_file(nullptr)
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
		if(_file != nullptr)
			close();

		_file = _openFile(fileName, openMode);
		return _file != nullptr;
	}
	
	int FileStream::close()
	{
		if(_file == nullptr)
			return -1;

		return FileSystem::CloseFile(_file);
	}

	int64 FileStream::getSize() const
	{
		if(!isOpen())
		{
			TRACE_WARNING("Trying to get size from an invalid file stream!");
			return 0;
		}
		return FileSystem::FileLength(_file);
	}


	int64 FileStream::read(void* buffer, uint32 size, uint32 count)
	{
		if(!isOpen()) 
		{
			TRACE_WARNING("Trying to read from an invalid file stream!");
			return -1; 
		}

		return FileSystem::Read(_file, buffer, size, count); 
	}

	int64 FileStream::write(const void* buffer, uint32 size, uint32 count)
	{
		if(!isOpen())
		{
			TRACE_WARNING("Trying to write to an invalid file stream!");
			return -1;
		}

		return FileSystem::Write(_file, buffer, size, count);
	}

	int64 FileStream::seek(uint64 position)
	{
		if(!isOpen())
		{
			TRACE_WARNING("Trying to seek in an invalid file stream!");
			return -1;
		}

		return FileSystem::Seek(_file, position);
	}

	int64 FileStream::tell() const
	{
		if(!isOpen())
		{
			TRACE_WARNING("Trying to tell in an invalid file stream!");
			return -1;
		}

		return FileSystem::Tell(_file);
	}

	// --------------------------------------------------------------------------------

	PHYSFS_File * FileStream::_openFile(
		const std::string & fileName, 
		const OpenMode & openMode)	const
	{
		switch(openMode)
		{
		case OpenMode::OPEN_READ:	return FileSystem::OpenFileRead(fileName);
		case OpenMode::OPEN_WRITE:	return FileSystem::OpenFileWrite(fileName);
		case OpenMode::OPEN_APPEND:	return FileSystem::OpenFileAppend(fileName);
		}
		return nullptr;
	}
}