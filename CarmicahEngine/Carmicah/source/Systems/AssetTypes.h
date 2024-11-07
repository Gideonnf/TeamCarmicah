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
#include "Math/Matrix3x3.h"

namespace Carmicah
{
	struct Primitive
	{
		std::vector<Vec2f> vtx;
		std::vector<Vec2f> texCoord;
		std::vector<GLushort> idx;
		GLenum drawMode{};
		GLuint drawCnt{};
	};
	struct DebugPrimitive
	{
		std::vector<Vec2f> vtx;
		GLenum drawMode{};
		GLuint drawCnt{};
	};
	struct BatchBuffer
	{
		GLuint vao{}, vbo{}, ebo{}, ibo{};
		const Primitive* pRef;
	};
	struct Shader
	{
		GLuint s;
	};
	//struct ImageTexture
	//{
	//	GLuint t;
	//};
	struct AnimAtlas
	{
		std::vector<std::pair<float, std::string>> anim;// MaxTime, TextureName
	};
	struct Texture
	{
		GLuint t;
		Mtx3x3f mtx;
	};
	struct FontTexture
	{
		GLuint t;
		Mtx3x3f mtx;
	};
	struct Font
	{
		struct FontChar
		{
			std::string	 texRef{};
			unsigned int width{}, height{};
			int			 xBearing{}, yBearing{};
			long		 advance{};
		};
		const unsigned char charOffset{ 32 }; // first 32 characters are not visable

		std::array<FontChar, 96> mFontMaps{};
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
	
		std::vector<Prefab> childList;
	};
	struct Scene
	{
		std::string sceneFile;
	};
	struct EngineConfig
	{
		int Width{}, Height{}, fontSize{}, maxNumTextures{}, batchRenderSize{};
		float maxImgDepth{}, minImgDepth{};
		std::string defaultScene{};
		std::string lastScene{};
		std::string defaultShader{};
		std::string assetLoc{};
	};
}

#endif