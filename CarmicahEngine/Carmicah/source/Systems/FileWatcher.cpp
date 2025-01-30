/* File Documentation -----------------------------------------------------------------------------
file:           FileWatcher.cpp

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

brief:          This file implements the FileWatcher class, which monitors a directory for file
				changes. It tracks file creation, modification, and deletion. The `Init` method
				initializes the directory watch, the `Update` method processes changes, and the
				`DestroyFile` method handles file removal from the watch list. This class integrates
				with the AssetManager to load or reload assets when changes are detected.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


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
				std::string fileExt = file.path().extension().string();
				if (fileExt != ".txt")
					assetMap.insert({ file.path().filename().stem().string(), File(file, file.path().string(), std::filesystem::last_write_time(file), FILE_CREATED)});
				//std::string fileExt = file.path().extension().string();


				// TODO: Find a way to pull out audio files being used from C# scripting side
				// for now we just load all audio
				if (fileExt == ".wav" || fileExt == ".ogg" || fileExt == ".mp3")
				{
					if (AssetManager::GetInstance()->LoadAsset(assetMap[file.path().filename().stem().string()]))
					{
						assetMap[file.path().filename().stem().string()].fileStatus = FILE_OK;
					}
				}
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
				std::string filePath = it->first;
				it = fileMap.erase(it);
				//Erase from the respective assetMap too

				AssetManager::GetInstance()->RemoveAsset(filePath);
				break;
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

	void FileWatcher::LoadSceneFiles(std::string const& sceneFile)
	{
		for (auto& asset : AssetManager::GetInstance()->enConfig.assetsToLoad[sceneFile])
		{
			if (assetMap.count(sceneFile) != 0)
			{
				if (assetMap[asset].fileStatus == FILE_CREATED)
				{
					if (AssetManager::GetInstance()->LoadAsset(assetMap[asset]))
					{
						assetMap[asset].fileStatus = FILE_OK;
						std::string fileExt = assetMap[asset].fileEntry.path().extension().string();

						// idk how else to load do and o since they have the same name the map will only store one
						if (fileExt == ".do")
						{
							const auto objectFile = assetMap[asset].fileEntry.path().parent_path() / (assetMap[asset].fileEntry.path().stem().string() + std::string(".o"));
							if (assetMap.count(objectFile.string()) != 0)
							{
								if (assetMap[objectFile.string()].fileStatus == FILE_CREATED)
								{
									AssetManager::GetInstance()->LoadAsset(assetMap[objectFile.string()]);
									assetMap[objectFile.string()].fileStatus = FILE_OK;
								}
							}
						}
						else if (fileExt == ".o")
						{
							const auto objectFile = assetMap[asset].fileEntry.path().parent_path() / (assetMap[asset].fileEntry.path().stem().string() + std::string(".do"));
							if (assetMap.count(objectFile.string()) != 0)
							{
								if (assetMap[objectFile.string()].fileStatus == FILE_CREATED)
								{
									AssetManager::GetInstance()->LoadAsset(assetMap[objectFile.string()]);
									assetMap[objectFile.string()].fileStatus = FILE_OK;
								}
							}
						}
					}
				}
			}
		}

		/*for (auto it = fileMap.begin(); it != fileMap.end(); ++it)
		{

		}*/
	}

	void FileWatcher::LoadSoundFiles()
	{
		std::filesystem::path directoryPath = AssetManager::GetInstance()->enConfig.assetLoc.c_str() + std::string("/Audio");
	}

	auto FileWatcher::DestroyFile(File file)
	{
		auto it = fileMap.find(file.fileName);
		fileMap.erase(it);
		return it;
	}
}