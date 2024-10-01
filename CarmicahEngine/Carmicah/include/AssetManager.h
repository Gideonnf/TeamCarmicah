#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <freetype/ft2build.h>
#include FT_FREETYPE_H
#include <FMOD/fmod.hpp>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include "Singleton.h"

namespace Carmicah
{
	class IAsset {
	public:
		virtual ~IAsset() = default;
	};

	template <typename T>
	class Asset : public IAsset
	{
	public:
		std::unordered_map<std::string, std::shared_ptr<IAsset>> mAssetMap;
	};

	template <typename T>
	struct BaseAsset {
		std::unordered_map<std::string, T> mAssetMap;
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

	class AssetManager : public Singleton<AssetManager>
	{
	public:
		
		// Data
		void LoadAll(const char*);
		void UnloadAll();

		//std::unordered_map<std::string, std::shared_ptr<BaseAsset>> mMapOfAssets;
		//
		//std::unordered_map<std::string, GLuint> mShaderPgms{};
		//std::unordered_map<std::string, Texture> mTextureMaps{};
		//std::unordered_map<std::string, Primitive> mPrimitiveMaps{};
		//std::unordered_map<std::string, std::string> mSceneFiles{};
		//std::unordered_map<std::string, std::string> mPrefabFiles{};
		FT_Library mFTLib;
		const unsigned int fontSize{ 36 };
		std::unordered_map<std::string, std::array<Carmicah::FontChar, 128>> mFontMaps{};

		// Audio
		const int maxChannels{ 32 };
		FMOD::System* mSoundSystem{};
		std::unordered_map<std::string, Audio> mSoundMap{};


		bool GetScene(std::string scene, std::string& filePath);

		//template<typename T>
		//bool TryGetAsset(Primitive&, const std::string&);
	private:

		// Graphics Assets
		GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile);
		void LoadObject(const std::string& objName, const std::string& modelFile);
		void LoadDebugObject(const std::string& objName, const std::string& modelFile);
		void LoadTexture(const std::string& textureName, const std::string& textureFile, const std::string& spriteSheetFile);
		void InitFontType();
		void LoadFont(const std::string& fontName, const std::string& fontLoc, const unsigned int& fontHeight);

		void ExportCircle(int numSlices, const std::string& modelFile);


		void InitSound();
		// Sound Assets
		void LoadSound(const std::string& soundName, std::string const& soundFile, bool b_isLoop);

		//void LoadScene()
	};
}
#endif