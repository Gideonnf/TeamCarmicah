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
#include <functional>
#include "Math/Matrix3x3.h"

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
	struct ImageTexture
	{
		GLuint t;
	};
	struct TextureAtlas
	{
		GLuint t;
		unsigned int slotWidth, slotHeight;
		struct Slot
		{
			unsigned int x, y;
			bool vacant, checked;
		};
		std::vector<Slot> spaces{};
	};
	struct AnimAtlas
	{
		std::vector<std::pair<float, std::string>> anim;// MaxTime, TextureName
	};
	struct Texture
	{
		GLuint t;
		Mtx3x3f mtx;
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
	
		std::vector<Prefab> childList;

		template <typename T>
		bool HasComponent() const
		{
			std::string typeName = typeid(T).name();
			for (const auto& [name, component] : mComponents)
			{
				if (typeName == name)
				{
					return true;
				}
			}
			return false;

		}


		template <typename T>
		T& GetComponent()
		{
			std::string typeName = typeid(T).name();
			auto it = mComponents.find(typeName);
			if (it != mComponents.end())
			{
				return std::any_cast<T&>(it->second);
			}
			throw std::runtime_error("Component not found!");

		}

		std::string GetName() const
		{
			return mName;
		}

		void ForPrefabChildren(Prefab& parentPrefab, const std::function<void(Prefab&)>& func)
		{
			if (parentPrefab.childList.size() > 0)
			{
				for (auto& child : parentPrefab.childList)
				{
					func(child);
				}
			}

		}
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