#ifndef AM_TEST_H
#define AM_TEST_H

#include <unordered_map>
#include "Singleton.h"
#include <any>
namespace CarmicahTesting
{
	class IAsset
	{
	public:
		virtual ~IAsset() = default;
	};

	template <typename T>
	class Asset : public IAsset
	{
	public:
		std::vector<T> mAssetList;
		std::unordered_map<std::string, unsigned int> mAssetMap;
	};

	struct Primitive
	{
		GLuint vaoid{};
		GLuint vboid{};
		GLenum drawMode{};
		GLuint drawCnt{};
	};
	struct Shader
	{
		GLuint s;
	};
	struct Texture
	{
		GLuint t;
		int width;
		int height;
		int bpt;

		int xSlices;
		int ySlices;
	};
	struct FontChar
	{
		unsigned int texID, width, height;
		int			 xBearing, yBearing;
		long		 advance;
	};
	struct Audio
	{
		bool isLoop;
		FMOD::Sound* sound;
	};
	struct Prefab
	{
		unsigned int mPrefabID;
		std::string mName;
		std::unordered_map<std::string, std::any> mComponents;
	};
	struct Scene
	{
		std::string sceneFile;
	};
	struct EngineConfig
	{
		int Width, Height, fontSize;
		std::string defaultScene;
		std::string lastScene;
		std::string defaultShader;
		std::string fontShader;
		std::string assetLoc;
	};

	//AssetManager.GetAsset<Prefab>(Duck);

	class AssetManagerPtrVersion : public Singleton<AssetManagerPtrVersion>
	{
	public:
		// since theres gonna be different assets, storing it as a generic void pointer can
		// ignore the type of asset and just store it as an Asset<T>
		// Then just static cast based on the type we're looking for
		// kinda the same as component manager except theres another level above component manager
		std::unordered_map<std::string, std::shared_ptr<IAsset>> mAssetTypeMap; // idk how to name this map

		template <typename T>
		std::shared_ptr<Asset<T>> GetAssetMap()
		{
			std::string assetType = typeid(T).name();

			// Doesnt exist in the map yet
			if (mAssetTypeMap.count(assetType) == 0)
			{
				assert("Asset type does not exist");
				//mAssetMap[assetType] = std::make_unique<Asset<T>>();
			}

			return std::static_pointer_cast<Asset<T>>(mAssetTypeMap[assetType]);
		}

		template <typename T>
		T& GetAsset(std::string name)
		{
			if (GetAssetMap<T>()->mAssetList.size() == 0)
				assert("Asset does not exist");
			if (GetAssetMap<T>()->mAssetMap.count(name) == 0)
				assert("Asset does not exist");
			
			return GetAssetMap<T>()->mAssetList[GetAssetMap<T>()->mAssetMap[name]];
		}

		template <typename T>
		bool AssetExist(std::string name)
		{
			if (GetAssetMap<T>()->mAssetList.size() == 0)
				return false;
			if (GetAssetMap<T>()->mAssetMap.count(name) == 0)
				return false;

			return true;
		}

		template <typename T>
		void RegisterAsset()
		{
			std::string assetType = typeid(T).name();

			if (mAssetTypeMap.count(assetType) == 0)
			{
				mAssetTypeMap[assetType] = std::make_shared<Asset<T>>();
			}
			else
			{
				//CM_CORE_ERROR("Asset being registered more than once");
				assert("Asset has been registered before");
			}
		}

		template <typename T>
		void AddAsset(std::string name, T& asset)
		{
			GetAssetMap<T>()->mAssetList.push_back(asset);
			GetAssetMap<T>()->mAssetMap[name] = GetAssetMap<T>()->mAssetList.size() - 1;
		}

		void LoadAll(const char* assetPath);
		void LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile);

	};
}


#endif