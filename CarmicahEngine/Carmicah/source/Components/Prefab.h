/* File Documentation -----------------------------------------------------------------------------
file:		Prefab.h

author:		Gideon Francis (100%)

email:      g.francis@digipen.edu

brief:      This file defines the PrefabData structure, which inherits from BaseComponent.
			It includes functionality to serialize and deserialize data related to prefabs,
			such as a reference to the prefab source and a placeholder for tracking modified
			components. The `mComponentsModified` vector is intended to track changes to
			components but is not yet fully implemented.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
--------------------------------------------------------------------------------------------------*/


#ifndef PREFAB_H
#define PREFAB_H
#include "BaseComponent.h"

namespace Carmicah
{
	struct PrefabData : BaseComponent<PrefabData>
	{
		unsigned int mPrefabRef; // Keep a ref to which prefab it was made from if any. If none then 0
		// If the component is modified, keep track of it here
		// We don't want to override any components that was modified with new prefab data.
		std::vector<std::string> mComponentsModified; //TODO: finish using this vector cause im not gonna do this atm


		PrefabData& DeserializeComponent(const rapidjson::Value& component) override
		{
			mPrefabRef = component["prefabRef"].GetUint();
			return *this;
		}

		void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
		{
			writer.String("prefabRef");
			writer.Uint(mPrefabRef);

		}
	};
}


#endif
