/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TransformSystem.h

 author:		YANG YUJIE(70%)
 co-author(s):	Gideon Francis(30%)

 email:			g.francis@digipen.edu
					y.yujie@digipen.edu

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

		/// <summary>
		/// Used to receive messages by the System Manager when sent a message from other systems
		/// </summary>
		/// <param name="msg">base message class so that the system can cast it to the right msg</param>
		void ReceiveMessage(Message* msg)  override;
		/// <summary>
		/// Calculates the transform of the entity when being updated
		/// </summary>
		/// <param name="entityID">Entity ID</param>
		/// <param name="parentID">Parent ID</param>
		/// <param name="ToWorld">If being parented back to world/scene</param>
		void CalculateTransform(Entity entityID, Entity parentID, bool ToWorld = false);
	};

}

#endif