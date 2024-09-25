#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <GLFW/glfw3.h>
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

		// Audio
		const int maxChannels = 32;
		FMOD::System* soundSystem;
		std::unordered_map<std::string, Audio> soundMap;

		//template<typename T>
		//bool TryGetAsset(Primitive&, const std::string&);
	private:

		// Graphics Assets
		GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile);
		void LoadObject(const std::string& objName, const std::string& modelFile);
		void LoadDebugObject(const std::string& objName, const std::string& modelFile);
		void LoadTexture(const std::string& textureName, const std::string& textureFile);
		
		void ExportCircle(int numSlices, const std::string& modelFile);


		void InitSound();
		// Sound Assets
		void LoadSound(const std::string& soundName, std::string const& soundFile, bool b_isLoop);

	};
}
#endif