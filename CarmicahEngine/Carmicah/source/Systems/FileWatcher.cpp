#include <pch.h>
#include "FileWatcher.h"
#include "AssetManager.h"
namespace Carmicah
{
	void FileWatcher::Init(std::string filePath)
	{
		for (auto& file : std::filesystem::recursive_directory_iterator(filePath)) 
		{
			if (!std::filesystem::is_directory(file))
			{
			//	File newFile;
			//	newFile.fileEntry = file;
			////	auto test = std::filesystem::directory_iterator(file);

			//	newFile.fileName = file.path().string();
			//	newFile.time = std::filesystem::last_write_time(file);
			//	newFile.fileStatus = FILE_CREATED;
			//	fileMap[file.path().string()] = newFile;

				fileMap.insert({ file.path().string(), File(file, file.path().string(), std::filesystem::last_write_time(file), FILE_CREATED) });

				//std::string fileExt = file.path().extension().string();

			}
		}

		filePathReference = filePath;
	}

	void FileWatcher::Update()
	{
		// Check if any files were deleted
		for (auto it = fileMap.begin(); it != fileMap.end(); ++it)
		{
			// The file no longer exist
			if (!std::filesystem::exists(it->first))
			{
				// Can call a function here to handle on erase of files

				it->second.fileStatus = FILE_DELETED;

				//it = fileMap.erase(it);
			}
			else if (it->second.fileStatus != FILE_OK)
			{
				if (it->second.fileStatus == FILE_CREATED)
				{
					if (AssetManager::GetInstance()->LoadAsset(it->second))
					{
						it->second.fileStatus = FILE_OK;
					}
				}
				else if (it->second.fileStatus == FILE_MODIFIED)
				{
					if (AssetManager::GetInstance()->LoadAsset(it->second, true))
					{
						it->second.fileStatus = FILE_OK;
					}
				}
			}
		}

		for (auto& file : std::filesystem::recursive_directory_iterator(filePathReference))
		{
			// Skip directory folders, only need actual asset files
			if (std::filesystem::is_directory(file))
			{
				continue;
			}
			// Get the current file's last write time
			auto lastWriteTime = std::filesystem::last_write_time(file);
			// If this file doesn't exist in the map, means its new
			if (fileMap.count(file.path().string()) == 0)
			{
				fileMap[file.path().string()].time = lastWriteTime;
				fileMap[file.path().string()].fileName = file.path().string();
				fileMap[file.path().string()].fileEntry = file;

				// New file
				fileMap[file.path().string()].fileStatus = FILE_CREATED;
			}
			else
			{
				// If the current copy of hte file has a different last write time
				if (fileMap[file.path().string()].time != lastWriteTime)
				{
					fileMap[file.path().string()].time = lastWriteTime;
					fileMap[file.path().string()].fileStatus = FILE_MODIFIED;
					// File modified
				}
			}
		//	if (fileMap.count(file.))
		}
	}

	auto FileWatcher::DestroyFile(File file)
	{
		auto it = fileMap.find(file.fileName);
		fileMap.erase(it);
		return it;
	}
}