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