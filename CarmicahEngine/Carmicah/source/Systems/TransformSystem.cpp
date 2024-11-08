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
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

			// Handle Entities transform
			if (!transform.notUpdated)
			{
				// if no parent
				if (transform.parent == 0)
				{
					Mtx33Identity(transform.worldSpace);
					transform.worldSpace.translateThis(transform.pos.x, transform.pos.y).rotDegThis(transform.rot).scaleThis(transform.scale.x, transform.scale.y);
					transform.localSpace = transform.worldSpace; // if no parent, local and world is the same
				}
				// have parent
				else
				{
					// get parent's transform
					Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(transform.parent);
					Mtx33Identity(transform.localSpace);
					transform.localSpace.translateThis(transform.pos.x, transform.pos.y).rotDegThis(transform.rot).scaleThis(transform.scale.x, transform.scale.y);
					transform.worldSpace = parentTransform.worldSpace * transform.localSpace;
				}
			}
		}
	}

	void TransformSystem::CalculateTransform(Entity entityID, Entity parentID, bool ToWorld)
	{
		Transform& entityTransform = ComponentManager::GetInstance()->GetComponent<Transform>(entityID);
		Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(parentID);

		if (ToWorld)
		{
			// Get back world transform pos
			entityTransform.pos.x = parentTransform.pos.x + entityTransform.pos.x;

			// get back world transform rot
			entityTransform.rot = parentTransform.rot + entityTransform.rot;

			// get back world transform scale
			entityTransform.scale.x = parentTransform.scale.x * entityTransform.scale.x;
			entityTransform.scale.y = parentTransform.scale.y * entityTransform.scale.y;

		}
		else
		{
			// get the change in position
			float deltaX = entityTransform.pos.x - parentTransform.pos.x;
			float deltaY = entityTransform.pos.y - parentTransform.pos.y;

			// convert parent rot to radians
			float rad = parentTransform.rot * (PI / 180.0f);

			float cosTheta = cos(rad);
			float sinTheta = sin(rad);

			// Calculate the new transform position
			entityTransform.pos.x = deltaX * cosTheta + deltaY * sinTheta;
			entityTransform.pos.y = -deltaX * sinTheta + deltaY * cosTheta;

			//entityTransform.scale /= parentTransform.scale; // doesntw ork for some reason
			entityTransform.scale.x = entityTransform.scale.x / parentTransform.scale.x;
			entityTransform.scale.y = entityTransform.scale.y / parentTransform.scale.y;

			// calculate the new rotation
			entityTransform.rot = entityTransform.rot - parentTransform.rot;
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