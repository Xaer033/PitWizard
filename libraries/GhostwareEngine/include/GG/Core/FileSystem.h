#pragma once

#include <Physfs/physfs.h>
#include <string>
#include <vector>

#include <GG/Core/Types.h>

namespace GG
{

	typedef PHYSFS_File FileHandle;
	typedef PHYSFS_ArchiveInfo ArchiveInfo;

	class FileSystem
	{
	public:
		static bool					Init(const char * argv0);
		static bool					Shutdown();

		static bool					GetSupportedArchiveTypes(
										std::vector<const ArchiveInfo*> & outArchiveTypeList);

		static const std::string	GetLastError();

		static const std::string	GetDirSeparator();
		
		static const std::string	GetBaseDirectory();
		static const std::string	GetUserDirectory();

		static const std::string	GetWriteDirectory();
		static bool					SetWriteDirectory(const std::string & directory);

		static bool					RemoveFromSearchPath(const std::string & oldDir);
		static bool					GetSearchPath(std::vector<std::string> & outSearchPathList);
		static bool					Mount(
										const std::string & newDir, 
										const std::string & mountPoint = "/", 
										bool appendToPath = false);

		static const std::string	GetMountPoint(const std::string & directory);

		static bool					MakeDirectory(const std::string & dirName);
		static bool					Delete(const std::string & fileName);
		static const std::string	GetRealDirectory(const std::string & fileName);

		static bool					GetFilesInDirectory(
										const std::string & directory, 
										std::vector<std::string> & outFileList);
		
		static bool					FileExists(const std::string & fileName);
		static bool					IsDirectory(const std::string & dirName);

		static void					PermitSymbolicLinks(bool allow);
		static bool					IsSymbolicLink(const std::string & fileName);
		static bool					IsSymbolicLinkPermitted();

		static int64				GetLastModTime(const std::string & fileName);


	/// File Operations
		static FileHandle *			OpenFileWrite(const std::string & fileName);
		static FileHandle *			OpenFileAppend(const std::string & fileName);
		static FileHandle *			OpenFileRead(const std::string & fileName);

		static bool					CloseFile(FileHandle * handle);

		static int64				Read(
										FileHandle * handle,
										void * buffer,
										uint32 objSize,
										uint32 objCount);

		static int64				Write(
										FileHandle * handle, 
										const void * buffer, 
										uint32 objSize, 
										uint32 objCount);

		static bool					EndOfFile(FileHandle * handle);

		static int64				Tell(FileHandle * handle);
		static bool					Seek(FileHandle * handle, uint64 position);
		
		static int64				FileLength(FileHandle * handle);
		static bool					SetFileBuffer(FileHandle * handle, uint64 bufferSize);
		static bool					FlushFile(FileHandle * handle);
	};
}
