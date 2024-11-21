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
		FILE_OK
	};

	struct File
	{
		std::filesystem::directory_entry fileEntry;
		std::string fileName;
		std::filesystem::file_time_type time;
		FileStatus fileStatus;
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