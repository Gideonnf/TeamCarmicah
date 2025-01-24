/* File Documentation -----------------------------------------------------------------------------
file:           AssetTypes.cpp

author:			Won Yu Xuan Rainne(100%)

email : won.m@digipen.edu

brief:          This file implements the Prefab class methods from AssetTypes. It includes methods
				to retrieve a prefab's name and ID and provides functionality for iterating over
				child prefabs through a callback function. This supports hierarchical prefab
				structures within the game engine.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


#include <pch.h>
#include "AssetTypes.h"

namespace Carmicah
{
	std::string Prefab::GetName() const
	{
		return mName;
	}

	void Prefab::ForPrefabChildren(Prefab& parentPrefab, const std::function<void(Prefab&)>& func)
	{
		if (parentPrefab.childList.size() > 0)
		{
			for (auto& child : parentPrefab.childList)
			{
				func(child);
			}
		}
	}

	unsigned int Prefab::GetID() const
	{
		return mPrefabID;
	}

	void Prefab::AddCollisionLayer(CollisionLayer layer)
	{
		if (HasComponent<Transform>())
		{
			GetComponent<Transform>().collisionMask |= layer;
		}
	}

	void Prefab::RemoveCollisionLayer(CollisionLayer layer)
	{
		if (HasComponent<Transform>())
		{
			GetComponent<Transform>().collisionMask &= ~layer;
		}
	}
}