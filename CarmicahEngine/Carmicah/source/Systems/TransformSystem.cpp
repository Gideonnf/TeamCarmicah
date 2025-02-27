/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TransformSystem.cpp

 author:		Won Yu Xuan Rainne(60%)
				YANG YUJIE (40%)

 email:			won.m@digipen.edu
				y.yujie@digien.edu

 brief:			Transform System. Header file that contains the function declarations of the transform system. In charge of updating the transform
				of entities when parenting/unparenting between entities and scene

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "TransformSystem.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Input/InputSystem.h"
#include "AssetManager.h"

namespace Carmicah
{
	void TransformSystem::Init()
	{
		SystemManager::GetInstance()->SetSignature<TransformSystem>(mSignature);

		// setting layers to 1 by default is done in serializing system when serializing config file
		for (int i = 0; i < MAX_LAYERS; ++i)
		{
			// set default all bits to 1
			layerArr[i] = AssetManager::GetInstance()->enConfig.savedLayerArr[i];
		}

		maxLayers = GetLayerIndex(CollisionLayer::TOTAL_LAYERS);
	}

	void TransformSystem::EnableLayerInteraction(CollisionLayer layer1, CollisionLayer layer2)
	{
		// set layer 1 to interact with layer 2
		int layerIndex = GetLayerIndex(layer1);
		layerArr[layerIndex] |= static_cast<uint32_t>(layer2);

		// set layer 2 to interact with layer 1
		layerIndex = GetLayerIndex(layer2);
		layerArr[layerIndex] |= static_cast<uint32_t>(layer1);

	}

	void TransformSystem::DisableLayerInteraction(CollisionLayer layer1, CollisionLayer layer2)
	{
		// Remove layer 2 interaction with layer 1
		int layerIndex = GetLayerIndex(layer1);
		layerArr[layerIndex] &= ~static_cast<uint32_t>(layer2);

		// remove layer 1 interaction with layer 2
		layerIndex = GetLayerIndex(layer2);
		layerArr[layerIndex] &= ~static_cast<uint32_t>(layer1);

	}

	int TransformSystem::GetLayerIndex(CollisionLayer layer)
	{
		/*return static_cast<int>(std::log2(static_cast<uint32_t>(layer)));*/
		uint32_t value = static_cast<uint32_t>(layer);
		int index = 0;
		while (value >>= 1) { ++index; }
		return index;
	}

	const uint32_t* TransformSystem::GetLayerMap() const
	{
		return layerArr;
	}

	int TransformSystem::GetMaxLayers() const
	{
		return maxLayers;
	}

	const char* TransformSystem::GetLayerName(CollisionLayer layer)
	{	
		switch (layer)
		{
		case CollisionLayer::DEFAULT:
			return "DEFAULT";

		case CollisionLayer::PLAYER:
			return "PLAYER";

		case CollisionLayer::ENEMIES:
			return "ENEMIES";
		case CollisionLayer::ENVIRONMENT:
			return "ENVIRONMENT";
		case CollisionLayer::PROJECTILE:
			return "PROJECTILE";
		case CollisionLayer::TRAPS:
			return "TRAPS";
		default:
			return "NULL";
		}
	}

	bool TransformSystem::CheckLayerInteraction(unsigned int layer1, unsigned int  layer2)
	{
		int layerIndex = GetLayerIndex(static_cast<CollisionLayer>(layer1));
		return layerArr[layerIndex] & static_cast<uint32_t>(layer2);

		//return false;
	}

	void TransformSystem::AddToTransformMap(Entity e)
	{
		if (ComponentManager::GetInstance()->HasComponent<Transform>(e))
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(e);
			const unsigned int& level = transform.GrandChildLevel();
			// Can't add to non-existant map value
			auto i = transformedMap.find(level);
			if (i == transformedMap.end())
			{
				transformedMap.emplace(level, std::unordered_set<unsigned int>());
				i = transformedMap.find(level);
			}

			// Adding part
			if (transformedMap[level].find(e) == transformedMap[level].end())
				transformedMap[level].emplace(e);
			else
				return;

			// Recursive loop
			for (auto child : transform.children)
			{
				ComponentManager::GetInstance()->GetComponent<Transform>(child).Update();
				AddToTransformMap(child);
			}
		}
		else if (ComponentManager::GetInstance()->HasComponent<UITransform>(e))
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<UITransform>(e);
			const unsigned int& level = transform.GrandChildLevel();
			// Can't add to non-existant map value
			auto i = transformedMap.find(level);
			if (i == transformedMap.end())
			{
				transformedMap.emplace(level, std::unordered_set<unsigned int>());
				i = transformedMap.find(level);
			}

			// Adding part
			if (transformedMap[level].find(e) == transformedMap[level].end())
				transformedMap[level].emplace(e);
			else
				return;

			// Recursive loop
			for (auto child : transform.children)
			{
				ComponentManager::GetInstance()->GetComponent<UITransform>(child).Update();
				AddToTransformMap(child);
			}
		}
	}

	void TransformSystem::Update()
	{
		for (auto& e : mEntitiesSet)
		{
			if (ComponentManager::GetInstance()->HasComponent<Transform>(e))
			{
				auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(e);
				if (transform.Updated())
					AddToTransformMap(e);
			}
			else if (ComponentManager::GetInstance()->HasComponent<UITransform>(e))
			{
				auto& transform = ComponentManager::GetInstance()->GetComponent<UITransform>(e);
				if (transform.Updated())
					AddToTransformMap(e);
			}
		}

		for (auto& set : transformedMap)
		{
			for(auto entity : set.second)
				UpdateTransform(entity);
		}
	}

	// Handle Entities transform
	void TransformSystem::UpdateTransform(Entity e)
	{
		if (ComponentManager::GetInstance()->HasComponent<Transform>(e))
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(e);
			//CM_CORE_INFO("entity updating: " + std::to_string(entity));
			// if no parent
			if (transform.ParentID() == 0)
			{
				Mtx33Identity(transform.rotTrans);
				transform.accumulatedScale = transform.Scale();
				transform.rotTrans.translateThis(transform.Pos()).rotDegThis(transform.Rot());
				transform.worldSpace = transform.rotTrans;
				transform.worldSpace.scaleThis(transform.CalcedRenderingScale() * transform.accumulatedScale);
			}
			// have parent
			else
			{
				// get parent's transform
				Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(transform.ParentID());

				transform.accumulatedScale = parentTransform.accumulatedScale * transform.Scale();
				transform.rotTrans = parentTransform.rotTrans;
				transform.rotTrans.translateThis(transform.Pos()).rotDegThis(transform.Rot());
				transform.worldSpace = transform.rotTrans;
				transform.worldSpace.scaleThis(transform.CalcedRenderingScale() * transform.accumulatedScale);

			}
		}
		else if(ComponentManager::GetInstance()->HasComponent<UITransform>(e))
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<UITransform>(e);
			//CM_CORE_INFO("entity updating: " + std::to_string(entity));
			// if no parent
			if (transform.ParentID() == 0)
			{
				Mtx33Identity(transform.rotTrans);
				transform.accumulatedScale = transform.Scale();
				transform.rotTrans.translateThis(transform.Pos()).rotDegThis(transform.Rot());
				transform.worldSpace = transform.rotTrans;
				transform.worldSpace.scaleThis(transform.CalcedRenderingScale() * transform.accumulatedScale);
			}
			// have parent
			else
			{
				// get parent's transform
				UITransform& parentTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(transform.ParentID());

				transform.accumulatedScale = parentTransform.accumulatedScale * transform.Scale();
				transform.rotTrans = parentTransform.rotTrans;
				transform.rotTrans.translateThis(transform.Pos()).rotDegThis(transform.Rot());
				transform.worldSpace = transform.rotTrans;
				transform.worldSpace.scaleThis(transform.CalcedRenderingScale() * transform.accumulatedScale);

			}
		}
	}

	// TODO: HOW TO DESTROY CHILDREN - Can't orphan since transform is gone by this call
	void TransformSystem::EntityDestroyed(Entity id)
	{
		UNUSED(id);
		//auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(id);

		//for (auto child : transform.children)
		//{
		//	ChangeParent(child, 0);
		//}
	}

	void TransformSystem::PostUpdate()
	{
		for (std::map<unsigned int, std::unordered_set<unsigned int>>::iterator set{ transformedMap.begin() }; set != transformedMap.end(); ++set)
		{
			for (std::unordered_set<unsigned int>::iterator entity{ set->second.begin() }; entity != set->second.end(); ++entity)
			{
				if(ComponentManager::GetInstance()->HasComponent<Transform>(*entity))
					ComponentManager::GetInstance()->GetComponent<Transform>(*entity).ResetUpdate();
				else if (ComponentManager::GetInstance()->HasComponent<UITransform>(*entity))
					ComponentManager::GetInstance()->GetComponent<UITransform>(*entity).ResetUpdate();
			}
			set->second.clear();
		}
	}
	
	void TransformSystem::ChangeParent(Entity entityID, Entity parentID)
	{
		if (ComponentManager::GetInstance()->HasComponent<Transform>(entityID))
		{
			Transform& entityTransform = ComponentManager::GetInstance()->GetComponent<Transform>(entityID);

			// No Parent
			if (parentID == 0 && entityTransform.ParentID() != 0)
			{
				Transform& oldParentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(entityTransform.ParentID());

				// Reverse Calc from Matrix
				entityTransform.Pos(entityTransform.rotTrans.m[6], entityTransform.rotTrans.m[7]);
				entityTransform.Scale(entityTransform.Scale() * oldParentTransform.accumulatedScale);
				entityTransform.Rot(atan2f(entityTransform.rotTrans.m[1], entityTransform.rotTrans.m[0]) * (180.0f / PI));
				// Done w/ calculations

				// Recalc Matrices
				Mtx33Identity(entityTransform.rotTrans);
				entityTransform.accumulatedScale = entityTransform.Scale();
				entityTransform.rotTrans.translateThis(entityTransform.Pos()).rotDegThis(entityTransform.Rot());
				entityTransform.worldSpace = entityTransform.rotTrans;
				entityTransform.worldSpace.scaleThis(entityTransform.CalcedRenderingScale() * entityTransform.accumulatedScale);

				// Clear old parents data
				oldParentTransform.children.erase(std::find(oldParentTransform.children.begin(), oldParentTransform.children.end(), entityID));
				entityTransform.SetParent(0, 0);
			}
			else
			{
				Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(parentID);

				//entityTransform.Pos(entityTransform.worldSpace.m[6] - parentTransform.worldSpace.m[6], entityTransform.worldSpace.m[7] - parentTransform.worldSpace.m[7]);
				{
					// Math'ed it out
					float c = parentTransform.rotTrans.m[0], s = parentTransform.rotTrans.m[1],
						x = entityTransform.worldSpace.m[6], y = entityTransform.worldSpace.m[7],
						a = parentTransform.worldSpace.m[6], b = parentTransform.worldSpace.m[7];
					float newX = ((x - a) / c + s / (c * c) * (y - b)) / (1 + s * s / (c * c)),
						newY = (y - b - s * newX) / c;
					entityTransform.Pos(newX, newY);
				}
				entityTransform.Rot(entityTransform.Rot() - atan2f(parentTransform.rotTrans.m[1], parentTransform.rotTrans.m[0]) * (180.0f / PI));
				entityTransform.Scale(entityTransform.Scale().x / parentTransform.accumulatedScale.x, entityTransform.Scale().y / parentTransform.accumulatedScale.y);

				// Update the accumulatedScale with THIS's data as well
				entityTransform.accumulatedScale = parentTransform.accumulatedScale * entityTransform.Scale();
				entityTransform.rotTrans = parentTransform.rotTrans;
				entityTransform.rotTrans.translateThis(entityTransform.Pos()).rotDegThis(entityTransform.Rot());
				entityTransform.worldSpace = entityTransform.rotTrans;
				entityTransform.worldSpace.scaleThis(entityTransform.CalcedRenderingScale() * entityTransform.accumulatedScale);

				entityTransform.SetParent(parentID, parentTransform.GrandChildLevel());
				parentTransform.children.push_back(entityID);
			}
		}
		else if (ComponentManager::GetInstance()->HasComponent<UITransform>(entityID))
		{
			UITransform& entityTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(entityID);

			// No Parent
			if (parentID == 0 && entityTransform.ParentID() != 0)
			{
				UITransform& oldParentTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(entityTransform.ParentID());

				// Reverse Calc from Matrix
				entityTransform.Pos(entityTransform.rotTrans.m[6], entityTransform.rotTrans.m[7]);
				entityTransform.Scale(entityTransform.Scale() * oldParentTransform.accumulatedScale);
				entityTransform.Rot(atan2f(entityTransform.rotTrans.m[1], entityTransform.rotTrans.m[0]) * (180.0f / PI));
				// Done w/ calculations

				// Recalc Matrices
				Mtx33Identity(entityTransform.rotTrans);
				entityTransform.accumulatedScale = entityTransform.Scale();
				entityTransform.rotTrans.translateThis(entityTransform.Pos()).rotDegThis(entityTransform.Rot());
				entityTransform.worldSpace = entityTransform.rotTrans;
				entityTransform.worldSpace.scaleThis(entityTransform.CalcedRenderingScale() * entityTransform.accumulatedScale);

				// Clear old parents data
				oldParentTransform.children.erase(std::find(oldParentTransform.children.begin(), oldParentTransform.children.end(), entityID));
				entityTransform.SetParent(0, 0);
			}
			else
			{
				UITransform& parentTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(parentID);

				//entityTransform.Pos(entityTransform.worldSpace.m[6] - parentTransform.worldSpace.m[6], entityTransform.worldSpace.m[7] - parentTransform.worldSpace.m[7]);
				{
					// Math'ed it out
					float c = parentTransform.rotTrans.m[0], s = parentTransform.rotTrans.m[1],
						x = entityTransform.worldSpace.m[6], y = entityTransform.worldSpace.m[7],
						a = parentTransform.worldSpace.m[6], b = parentTransform.worldSpace.m[7];
					float newX = ((x - a) / c + s / (c * c) * (y - b)) / (1 + s * s / (c * c)),
						newY = (y - b - s * newX) / c;
					entityTransform.Pos(newX, newY);
				}
				entityTransform.Rot(entityTransform.Rot() - atan2f(parentTransform.rotTrans.m[1], parentTransform.rotTrans.m[0]) * (180.0f / PI));
				entityTransform.Scale(entityTransform.Scale().x / parentTransform.accumulatedScale.x, entityTransform.Scale().y / parentTransform.accumulatedScale.y);

				// Update the accumulatedScale with THIS's data as well
				entityTransform.accumulatedScale = parentTransform.accumulatedScale * entityTransform.Scale();
				entityTransform.rotTrans = parentTransform.rotTrans;
				entityTransform.rotTrans.translateThis(entityTransform.Pos()).rotDegThis(entityTransform.Rot());
				entityTransform.worldSpace = entityTransform.rotTrans;
				entityTransform.worldSpace.scaleThis(entityTransform.CalcedRenderingScale() * entityTransform.accumulatedScale);

				entityTransform.SetParent(parentID, parentTransform.GrandChildLevel());
				parentTransform.children.push_back(entityID);
			}
		}

	}

	void TransformSystem::ReceiveMessage(Message* msg)
	{
		// If its the correct message type
		if (msg->mMsgType == MSG_UPDATETRANSFORM)
		{
			// NOTE: This only works for normal world transform not UITransform

			UpdateTransformMessage* castedMsg = dynamic_cast<UpdateTransformMessage*>(msg);
			// Update the local transform based on parent position

			// Update scale
			if (ComponentManager::GetInstance()->HasComponent<Transform>(castedMsg->mEntityID))
			{
				Transform& entityTransform = ComponentManager::GetInstance()->GetComponent<Transform>(castedMsg->mEntityID);

				// Its being parented back to the scene
				// so we have to use its old's parent ID and reset it
				// if the entity transform parent is already 0, that means its already in world pos
				// so no calculation needed
				if (castedMsg->mParentID == 0 && entityTransform.ParentID() != 0)
				{
					ChangeParent(castedMsg->mEntityID, 0);
				}
				// If the original parent is not the scene
				// means it has a new parent that isnt the scene
				// so it isnt calculating world position
				else if (castedMsg->mParentID != 0)
				{
					ChangeParent(castedMsg->mEntityID, castedMsg->mParentID);
				}
				
			}
			else if (ComponentManager::GetInstance()->HasComponent<UITransform>(castedMsg->mEntityID))
			{
				UITransform& entityTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(castedMsg->mEntityID);
				// Its being parented back to the scene
				// so we have to use its old's parent ID and reset it
				// if the entity transform parent is already 0, that means its already in world pos
				// so no calculation needed
				if (castedMsg->mParentID == 0 && entityTransform.ParentID() != 0)
				{
					ChangeParent(castedMsg->mEntityID, 0);
				}
				// If the original parent is not the scene
				// means it has a new parent that isnt the scene
				// so it isnt calculating world position
				else if (castedMsg->mParentID != 0)
				{
					ChangeParent(castedMsg->mEntityID, castedMsg->mParentID);
				}
				
			}
		}
	}
}