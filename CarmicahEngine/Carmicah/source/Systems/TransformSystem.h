#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "../ECS/GameObject.h"
#include "../ECS/BaseSystem.h"

namespace Carmicah
{
	class TransformSystem : public BaseSystem
	{
	public:
		void Init();
		void Update();

		void ReceiveMessage(Message* msg)  override;
		void CalculateTransform(Entity entityID, Entity parentID, bool ToWorld = false);
		void CalculateChildren(Entity id, Entity parentID);
	};

}

#endif