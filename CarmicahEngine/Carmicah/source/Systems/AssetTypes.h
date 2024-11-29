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
#include "../ECS/ECSTypes.h"

namespace Carmicah
{
	struct BasePrimitive
	{
		static GLuint uidCount;
		GLuint uid;
		std::vector<Vec2f> vtx;
		GLenum drawMode{};
		GLuint drawCnt{};
	};
	
	struct Primitive : BasePrimitive
	{
		std::vector<Vec2f> texCoord;
		std::vector<GLushort> idx;
	};
	struct BatchBuffer						// Used exclusively in BaseGraphicsSystem, but Data is cleared from AssetManager
	{
		struct BBuffer
		{
			GLuint vao{}, vbo{};
		};

		GLuint ebo{}, ibo{};				// Indexing Helpers
		unsigned int objCount{};			// Keeps track of the Last "open space"
		bool isDebug{};						// Keep track of which render method to use
		const BasePrimitive* pRef;			// Primitive Ref for this buffer
		std::queue<unsigned int> freeData;	// Keeps track of any opened up slots in the buffer
		std::vector<BBuffer> buffer;		// Access the buffer directly
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
	
	const unsigned char charOffset{ 32 }; // first 32 characters are not visable

	struct Font
	{
		struct FontChar
		{
			std::string	 texRef{};
			unsigned int width{}, height{};
			int			 xBearing{}, yBearing{};
			long		 advance{};
		};

		std::array<FontChar, 96> mFontMaps{};
	};

	struct Prefab
	{
		unsigned int mPrefabID;
		std::string mName;
		std::unordered_map<std::string, std::any> mComponents;
		std::vector<std::string> mDeletedComponents;
		//std::unordered_map<std::string, std::any> mDeletedComponents;
	
		std::vector<Prefab> childList;

		/*
			Each prefab keep track of which entity it is watching
			A parent prefab will keep track of the parent object
			A child prefab will keep track of the child object

			that way if only the child is modified, then only the child prefab has to be changed
			reduce the numebr of loops and checks it has to go through
		*/
		// Keep track of entities made using this prefab
		//std::vector<Entity> entityWatcher;

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

		// idk if these will work/compile until its used and i cant use it atm cause idk how to open prefab editor
		template <typename T>
		void AddComponent()
		{
			T component{};
			mComponents.insert({ typeid(T).name(), std::make_any<T>(component)});
		}

		template <typename T>
		void RemoveComponent()
		{
			std::string typeName = typeid(T).name();
			auto it = mComponents.find(typeName);
			if (it != mComponents.end())
			{
				mDeletedComponents.push_back(typeName);
				mComponents.erase(typeName);
			}
		}

		std::string GetName() const;

		unsigned int GetID() const;

		void ForPrefabChildren(Prefab& parentPrefab, const std::function<void(Prefab&)>& func);
	};
	struct Scene
	{
		std::string sceneFile;
	};
	struct EngineConfig
	{
		int Width{}, Height{}, fontSize{}, maxNumTextures{}, batchRenderSize{}, maxTexSize{};
		float maxImgDepth{}, minImgDepth{};
		std::string defaultScene{};
		std::string lastScene{};
		std::string defaultShader{}, debugShader{}, fontShader{};
		std::string assetLoc{};
	};
}

#endif