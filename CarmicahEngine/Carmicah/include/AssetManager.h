#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <GLFW/glfw3.h>
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
	};

	class AssetManager : public Singleton<AssetManager>
	{
	public:
		
		// Data
		
		void LoadAll();
		void UnloadAll();

		std::unordered_map<std::string, GLuint> shaderPgms{};
		std::unordered_map<std::string, GLuint> textureMaps{};
		std::unordered_map<std::string, Primitive> primitiveMaps{};

		//template<typename T>
		//bool TryGetAsset(Primitive&, const std::string&);
	private:

		const std::filesystem::path directoryPath = "";

		GLuint LoadShader(const std::string& shaderName, const std::string& vertFile, const std::string& fragFile);
		void LoadObject(const std::string& objName, const std::string& modelFile);
		void LoadDebugObject(const std::string& objName, const std::string& modelFile);
		void LoadTexture(const std::string& textureName, const std::string& textureFile, const GLuint& width, const GLuint& height, const GLuint& bpt);
		void ExportCircle(int numSlices, const std::string& modelFile);

	};
}
#endif