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

}



