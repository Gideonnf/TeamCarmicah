#ifndef PREFAB_H
#define PREFAB_H
#include "BaseComponent.h"

namespace Carmicah
{
	struct PrefabData : BaseComponent<PrefabData>
	{
		unsigned int mPrefabRef; // Keep a ref to which prefab it was made from if any. If none then 0

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
