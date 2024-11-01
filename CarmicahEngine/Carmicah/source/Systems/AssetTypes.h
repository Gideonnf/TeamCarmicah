/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			AssetTypes.h

 author:		Won Yu Xuan Rainne(77.5%)
 co-author(s):	Gideon Francis(15%)
				YANG YUJIE(7.5%)

 email:			won.m@digipen.edu

 brief:			Asset Types holds the data type for the different assets

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef ASSET_TYPES_H
#define ASSET_TYPES_H

#include <GLFW/glfw3.h>
#include <FMOD/fmod.hpp>
#include <string>
#include <unordered_map>
#include <any>

namespace Carmicah
{
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
	struct Font
	{
		struct FontChar
		{
			unsigned int texID, width, height;
			int			 xBearing, yBearing;
			long		 advance;
		};

		std::array<FontChar, 128> mFontMaps;
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
		std::vector<std::any> childList;
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
}

#endif