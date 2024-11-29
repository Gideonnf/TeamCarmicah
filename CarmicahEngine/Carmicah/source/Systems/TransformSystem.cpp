/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        TransformSystem.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:     Transform System. Header file that contains the function declarations of the transform system. Incharge of updating the transform
				of entities when parenting/unparenting between entities and scene

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include "TransformSystem.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

namespace Carmicah
{
	void TransformSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());

		SystemManager::GetInstance()->SetSignature<TransformSystem>(mSignature);
	}

	void TransformSystem::Update()
	{
		for (auto& entity : mEntitiesSet)
		{
			UpdateTransform(entity);
		}
	}

	void TransformSystem::UpdateTransform(Entity entity)
	{
		auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

		// Handle Entities transform
		if (transform.Updated())
		{
			//CM_CORE_INFO("entity updating: " + std::to_string(entity));
			// if no parent
			if (transform.parent == 0)
			{
				Mtx33Identity(transform.worldSpace);
				transform.worldSpace.translateThis(transform.Pos()).rotDegThis(transform.Rot()).scaleThis(transform.Scale());
				transform.localSpace = transform.worldSpace; // if no parent, local and world is the same
				transform.WorldPos(transform.worldSpace.m20, transform.worldSpace.m21);

			}
			// have parent
			else
			{
				// get parent's transform
				Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(transform.parent);
				
				//float localX = transform.WorldPos().x - parentTransform.worldSpace.m02;
				//float localY = transform.WorldPos().y - parentTransform.worldSpace.m12;

				//float inverseRotX = parentTransform.worldSpace.m00 * localX + parentTransform.worldSpace.m10 * localY;
				//float inverseRotY = parentTransform.worldSpace.m01 * localX + parentTransform.worldSpace.m11 * localY;

				//transform.Pos(inverseRotX, inverseRotY);
				CM_CORE_INFO("Updating entity : " + std::to_string(entity));

				//CM_CORE_INFO("localPos X : " + std::to_string(inverseRotX) + ", localPos Y : " + std::to_string(inverseRotY));

				Mtx33Identity(transform.localSpace);
				Mtx33Identity(transform.worldSpace);

				transform.localSpace.translateThis(transform.Pos()).rotDegThis(transform.Rot()).scaleThis(transform.Scale());
				transform.worldSpace = parentTransform.worldSpace * transform.localSpace;
				//if (transform.worldUpdate)
				//{
				//	transform.worldSpace.translateThis(transform.WorldPos()).rotDegThis(transform.WorldRot()).scaleThis(transform.Scale());

				//	transform.worldUpdate = false;
				//}
				//else
				//{

				//}

				//transform.WorldPos(transform.worldSpace.m20, transform.worldSpace.m21);

			}

			// Update child transform
			if (transform.children.size() != 0)
			{
				for (auto it : transform.children)
				{
					auto& childTransform = ComponentManager::GetInstance()->GetComponent<Transform>(it);
					childTransform.Update(); // set the notUpdate flag to false for children objects
					//UpdateTransform(entity); // then update their transform
				}
			}
		}

	}

	void TransformSystem::PostUpdate()
	{
		for (auto& entity : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);
			transform.ResetUpdate();
		}
	}

	void TransformSystem::CalculateTransform(Entity entityID, Entity parentID, bool ToWorld)
	{
		Transform& entityTransform = ComponentManager::GetInstance()->GetComponent<Transform>(entityID);
		Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(parentID);

		if (ToWorld)
		{
			// Get back world transform pos
			entityTransform.PosX(parentTransform.Pos().x + entityTransform.Pos().x);
			entityTransform.PosY(parentTransform.Pos().y + entityTransform.Pos().y);

			entityTransform.WorldPos(entityTransform.Pos() + parentTransform.Pos());

			// get back world transform rot
			entityTransform.Rot(parentTransform.Rot() + entityTransform.Rot());

			// get back world transform scale
			entityTransform.Scale(parentTransform.Scale().x * entityTransform.Scale().x, parentTransform.Scale().y * entityTransform.Scale().y);

		}
		else
		{
			// get the change in position
			float deltaX = entityTransform.Pos().x - parentTransform.Pos().x;
			float deltaY = entityTransform.Pos().y - parentTransform.Pos().y;

			// convert parent rot to radians
			float rad = parentTransform.Rot() * (PI / 180.0f);

			float cosTheta = cos(rad);
			float sinTheta = sin(rad);

			// Calculate the new transform position
			entityTransform.Pos(deltaX * cosTheta + deltaY * sinTheta, -deltaX * sinTheta + deltaY * cosTheta);

			//entityTransform.scale /= parentTransform.scale; // doesntw ork for some reason
			entityTransform.Scale(entityTransform.Scale().x / parentTransform.Scale().x, entityTransform.Scale().y / parentTransform.Scale().y);

			// calculate the new rotation
			entityTransform.Rot(entityTransform.Rot() - parentTransform.Rot());

			entityTransform.WorldPos(entityTransform.Pos() + parentTransform.Pos());
		}
	}

	void TransformSystem::CalculateChildren(Entity id, Entity parentID)
	{
		Transform& entityTransform = ComponentManager::GetInstance()->GetComponent<Transform>(id);

		CalculateTransform(id, parentID);

		// If it has children as well, then calculate for that
		if (entityTransform.children.size() > 0)
		{
			for (auto& it : entityTransform.children)
			{
				CalculateChildren(it, id);
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
				// if the entity transform parent is already 0, that meants its already in world pos
				// so no calculation needed
				if (castedMsg->mParentID == 0 && entityTransform.parent != 0)
				{
					CalculateTransform(castedMsg->mEntityID, entityTransform.parent, true);
				}
				// If the original parent is not the scene
				// means it has a new parent that isnt the scene
				// so it isnt calculating world position
				else if (castedMsg->mParentID != 0)
				{
					CalculateTransform(castedMsg->mEntityID, castedMsg->mParentID);
				}

				// It has children
				if (entityTransform.children.size() > 0)
				{
					for (auto& it : entityTransform.children)
					{
						CalculateChildren(it, castedMsg->mEntityID);
					}
				}
			}
			else if (ComponentManager::GetInstance()->HasComponent<UITransform>(castedMsg->mEntityID))
			{
				UITransform& entityTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(castedMsg->mEntityID);

				entityTransform.parent = castedMsg->mParentID;
			}
		}
	}
}