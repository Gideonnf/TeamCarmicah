/* File Documentation -----------------------------------------------------------------------------
file:           FileWatcher.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

brief:          This file defines the FileWatcher class, which monitors a directory for changes
				to files. It includes functionality to track file creation, modification, and
				deletion. The File structure represents individual files with their status and
				metadata, while the FileWatcher class maintains a map of files and provides methods
				for initialization, updating the watch state, and handling file destruction.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


#ifndef FILE_WATCHER_H
#define FILE_WATCHER_H
#include <filesystem>
#include <chrono>
namespace Carmicah
{
	enum FileStatus
	{
		FILE_CREATED,
		FILE_MODIFIED,
		FILE_DELETED,
		FILE_IGNORE,
		FILE_OK
	};

	struct File
	{
		std::filesystem::directory_entry fileEntry;
		std::string fileName;
		std::filesystem::file_time_type time;
		FileStatus fileStatus;

		File() : fileStatus(FILE_IGNORE) {}

		File(std::filesystem::directory_entry entry, std::string name, std::filesystem::file_time_type ttype, FileStatus status) :
			fileEntry(entry), fileName(name), time(ttype), fileStatus(status) {}
	};

	class FileWatcher
	{
	public:
		std::string filePathReference;
		std::unordered_map<std::string, File> fileMap;
		void Init(std::string filePath);
		auto DestroyFile(File file);
		void Update();

	};
}

#endif