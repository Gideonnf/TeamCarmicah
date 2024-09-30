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
	struct BaseAsset {};
	struct Primitive : BaseAsset
	{
		GLuint vaoid{};
		GLuint vboid{};
		GLenum drawMode{};
		GLuint drawCnt{};
	};
	struct Shader : BaseAsset
	{
		GLuint s;
	};
	struct Texture : BaseAsset
	{
		GLuint t;
		int width;
		int height;
		int bpt;

		int xSlices;
		int ySlices;
	};
	struct FontChar : BaseAsset
	{
		unsigned int texID, width, height;
		int			 xBearing, yBearing;
		long		 advance;
	};
	struct Audio : BaseAsset
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

		std::unordered_map<std::string, GLuint> shaderPgms{};
		std::unordered_map<std::string, Texture> textureMaps{};
		std::unordered_map<std::string, Primitive> primitiveMaps{};
		FT_Library ftLib;
		const unsigned int fontSize{ 36 };
		std::unordered_map<std::string, std::array<Carmicah::FontChar, 128>> fontMaps{};

		// Audio
		const int maxChannels{ 32 };
		FMOD::System* soundSystem{};
		std::unordered_map<std::string, Audio> soundMap{};


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

	};
}
#endif