
#include "FileSystem.h"
#include <Physfs/physfs.h>
#include <string>
#include <vector>

namespace GG
{
	bool FileSystem::Init(const char * argv0)
	{
		return PHYSFS_init(argv0) != 0;
	}

	bool FileSystem::Shutdown()
	{
		return PHYSFS_deinit() != 0;
	}

	bool FileSystem::GetSupportedArchiveTypes(std::vector<const ArchiveInfo*> & outArchiveTypeList)
	{
		const PHYSFS_ArchiveInfo **i = PHYSFS_supportedArchiveTypes();
		if(i == nullptr) 
			return false;

		for(; *i != nullptr; i++)
			outArchiveTypeList.push_back(*i);

		return true;
	}

	const std::string FileSystem::GetLastError()
	{
		return std::string(PHYSFS_getLastError());
	}

	const std::string FileSystem::GetDirSeparator()
	{
		return PHYSFS_getDirSeparator();
	}

	const std::string FileSystem::GetBaseDirectory()
	{
		return PHYSFS_getBaseDir();
	}

	const std::string FileSystem::GetUserDirectory()
	{
		return PHYSFS_getUserDir();
	}

	const std::string FileSystem::GetWriteDirectory()
	{
		return PHYSFS_getWriteDir();
	}

	bool FileSystem::SetWriteDirectory(const std::string & directory)
	{
		return PHYSFS_setWriteDir(directory.c_str()) != 0;
	}

	bool FileSystem::RemoveFromSearchPath(const std::string & oldDir)
	{
		return PHYSFS_removeFromSearchPath(oldDir.c_str()) != 0;
	}

	bool FileSystem::GetSearchPath(std::vector<std::string> & outSearchPathList)
	{
		char ** i = PHYSFS_getSearchPath();
		if(i == nullptr)
			return false;

		for(; *i != nullptr; ++i)
			outSearchPathList.push_back(std::string(*i));
		
		PHYSFS_freeList(i);
		return true;
	}

	bool FileSystem::Mount(
		const std::string & newDir,
		const std::string & mountPoint,
		bool appendToPath)
	{
		return PHYSFS_mount(newDir.c_str(), mountPoint.c_str(), (int)appendToPath) != 0;
	}

	const std::string FileSystem::GetMountPoint(const std::string & directory)
	{
		return PHYSFS_getMountPoint(directory.c_str());
	}
	
	bool FileSystem::MakeDirectory(const std::string & dirName)
	{
		return PHYSFS_mkdir(dirName.c_str()) != 0;
	}

	bool FileSystem::Delete(const std::string & fileName)
	{
		return PHYSFS_delete(fileName.c_str()) != 0;
	}

	const std::string GetRealDirectory(const std::string & fileName)
	{
		return PHYSFS_getRealDir(fileName.c_str());
	}

	bool FileSystem::GetFilesInDirectory(
		const std::string & directory,
		std::vector<std::string> & outFileList)
	{
		char ** i = PHYSFS_enumerateFiles(directory.c_str());
		if(i == nullptr)
			return false;

		for(; *i != nullptr; ++i)
			outFileList.push_back(std::string(*i));

		PHYSFS_freeList(i);

		return true;
	}

	bool FileSystem::FileExists(const std::string & fileName)
	{
		return PHYSFS_exists(fileName.c_str()) != 0;
	}

	bool FileSystem::IsDirectory(const std::string & dirName)
	{
		return PHYSFS_isDirectory(dirName.c_str()) != 0;
	}

	void FileSystem::PermitSymbolicLinks(bool allow)
	{
		PHYSFS_permitSymbolicLinks((int)allow);
	}

	bool FileSystem::IsSymbolicLink(const std::string & fileName)
	{
		return PHYSFS_isSymbolicLink(fileName.c_str()) != 0;
	}
	
	bool FileSystem::IsSymbolicLinkPermitted()
	{
		return PHYSFS_symbolicLinksPermitted() != 0;
	}

	int64 GetLastModeTime(const std::string & fileName)
	{
		return PHYSFS_getLastModTime(fileName.c_str());
	}

/// File Operations
	FileHandle * FileSystem::OpenFileWrite(const std::string & fileName)
	{
		return PHYSFS_openWrite(fileName.c_str());
	}

	FileHandle * FileSystem::OpenFileAppend(const std::string & fileName)
	{
		return PHYSFS_openAppend(fileName.c_str());
	}

	FileHandle * FileSystem::OpenFileRead(const std::string & fileName)
	{
		return PHYSFS_openRead(fileName.c_str());
	}
				 
	bool		 FileSystem::CloseFile(FileHandle * handle)
	{
		return PHYSFS_close(handle) != 0;
	}
				
	int64 FileSystem::Read(
			FileHandle * handle,
			void * buffer,
			uint32 objSize,
			uint32 objCount)
	{
		return PHYSFS_read(handle, buffer, objSize, objCount);
	}
				
	int64 FileSystem::Write(
				FileHandle * handle,
				const void * buffer,
				uint32 objSize,
				uint32 objCount)
	{
		return PHYSFS_write(handle, buffer, objSize, objCount);
	}
				
	bool FileSystem::EndOfFile(FileHandle * handle)
	{
		return PHYSFS_eof(handle) != 0;
	}
				 
	int64 FileSystem::Tell(FileHandle * handle)
	{
		return PHYSFS_tell(handle);
	}
	bool FileSystem::Seek(FileHandle * handle, uint64 position)
	{
		return PHYSFS_seek(handle, position) != 0;
	}
			
	int64	FileSystem::FileLength(FileHandle * handle)
	{
		return PHYSFS_fileLength(handle);
	}

	bool	FileSystem::SetFileBuffer(FileHandle * handle, uint64 bufferSize)
	{
		return PHYSFS_setBuffer(handle, bufferSize) != 0;
	}

	bool	FileSystem::FlushFile(FileHandle * handle)
	{
		return PHYSFS_flush(handle) != 0;
	}
}