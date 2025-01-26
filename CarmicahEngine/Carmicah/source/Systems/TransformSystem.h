/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TransformSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Transform System. Header file that contains the function declarations of the transform system. Incharge of updating the transform
				of entities when parenting/unparenting between entities and scene

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include "../ECS/GameObject.h"
#include "../ECS/BaseSystem.h"
#include <map>
#include <unordered_set>

namespace Carmicah
{
	class TransformSystem : public BaseSystem
	{
	private:
		std::map<unsigned int, std::unordered_set<unsigned int>> transformedMap{};
		uint32_t layerArr[MAX_LAYERS];
		int maxLayers;
	public:
		/// <summary>
		/// Initializes the transform system by registering the components
		/// </summary>
		void Init();
		/// <summary>
		/// If any entities are updated, then recalculate their local and world space matrix transform used in graphics
		/// </summary>
		void Update();
		/// <summary>
		/// If any entities are updated, then reset their update flag
		/// </summary>
		void PostUpdate();
		
		void AddToTransformMap(Entity e);

		void UpdateTransform(Entity e);

		void EntityDestroyed(Entity id);

		/// <summary>
		/// Used to receive messages by the System Manager when sent a message from other systems
		/// </summary>
		/// <param name="msg">base message class so that the system can cast it to the right msg</param>
		void ReceiveMessage(Message* msg)  override;
		/// <summary>
		/// Calculates the transform of the entity when the parent is being updated
		/// </summary>
		/// <param name="entityID">Entity ID</param>
		/// <param name="parentID">Parent ID</param>
		void ChangeParent(Entity entityID, Entity parentID);

		void EnableLayerInteraction(CollisionLayer layer1, CollisionLayer layer2);

		void DisableLayerInteraction(CollisionLayer layer1, CollisionLayer layer2);

		int GetLayerIndex(CollisionLayer layer);

		int GetMaxLayers() const;

		const uint32_t* GetLayerMap() const;
	};

}

#endif