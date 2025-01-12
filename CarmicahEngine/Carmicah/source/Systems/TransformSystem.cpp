/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TransformSystem.cpp
  
 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			Transform System. Header file that contains the function declarations of the transform system. Incharge of updating the transform
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

namespace Carmicah
{
	void TransformSystem::Init()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Transform>());

		SystemManager::GetInstance()->SetSignature<TransformSystem>(mSignature);
	}

	void TransformSystem::AddToTransformMap(Entity e)
	{
		auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(e);
		// Can't add to non-existant map value
		auto i = transformedMap.find(transform.grandChildLvl);
		if (i == transformedMap.end())
		{
			transformedMap.emplace(transform.grandChildLvl, std::unordered_set<unsigned int>());
			i = transformedMap.find(transform.grandChildLvl);
		}

		// Adding part
		if (transformedMap[transform.grandChildLvl].find(e) == transformedMap[transform.grandChildLvl].end())
			transformedMap[transform.grandChildLvl].emplace(e);
		else
			return;

		// Recursive loop
		for (auto child : transform.children)
		{
			ComponentManager::GetInstance()->GetComponent<Transform>(child).Update();
			AddToTransformMap(child);
		}
	}

	void TransformSystem::Update()
	{
		for (auto& e : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(e);
			if (transform.Updated())
				AddToTransformMap(e);
		}

		for (auto& set : transformedMap)
		{
			for(auto entity : set.second)
				UpdateTransform(entity);
		}

		if (transformedMap.size() > 0)
		{
			transformedMap.clear();
		}

		//if(Input.IsKeyPressed(KEY_B))
		//for (auto& e : mEntitiesSet)
		//{
		//	auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(e);
		//	if (transform.parent != 0)
		//	{
		//		CalculateTransform(e, transform.parent, true);
		//	}
		//}
		
	}

	// Handle Entities transform
	void TransformSystem::UpdateTransform(Entity e)
	{
		auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(e);
		//CM_CORE_INFO("entity updating: " + std::to_string(entity));
		// if no parent
		if (transform.parent == 0)
		{
			transform.GetPos() += transform.GetUpdateAbsPos();
			transform.GetUpdateAbsPos() = Vec2f::zero();

			Mtx33Identity(transform.worldSpace);
			transform.worldSpace.translateThis(transform.Pos()).rotDegThis(transform.Rot()).scaleThis(transform.Scale());
			transform.localSpace = transform.worldSpace; // if no parent, local and world is the same
		}
		// have parent
		else
		{
			// get parent's transform
			Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(transform.parent);

			Mtx3x3f parentInvMtx{};
			float det{};
			Mtx33Inverse(&parentInvMtx, &det, parentTransform.worldSpace);

			Vec2f parentScale;
			Vec2f parentTranslation;
			float parentRotation;

			//if (det < 0)
			//{
			//	parentInvMtx.m20 = -parentInvMtx.m20;
			//}
			//else
			parentInvMtx.m20 = parentInvMtx.m21 = 0.f;
			transform.GetPos() += parentInvMtx * transform.GetUpdateAbsPos();

			transform.GetUpdateAbsPos() = Vec2f::zero();



			parentScale.x = Vec2f(parentTransform.worldSpace.m00, parentTransform.worldSpace.m10).length();
			parentScale.y = Vec2f(parentTransform.worldSpace.m01, parentTransform.worldSpace.m11).length();
			parentTranslation = Vec2f(parentTransform.worldSpace.m20, parentTransform.worldSpace.m21);
			parentRotation = atan2f(parentTransform.worldSpace.m10, parentTransform.worldSpace.m00);
			CM_CORE_INFO("Entity Scale : ({},{}), Entity Position : ({}, {})", parentScale.x, parentScale.y, parentTranslation.x, parentTranslation.y);

			//if (parentTransform.worldSpace.m00 < 0) parentScale.x *= -1;
			//if (parentTransform.worldSpace.m11 < 0) parentScale.y *= -1;

			//Vec2f adjustedLocalPos = transform.Pos();
			//if (parentScale.x < 0) adjustedLocalPos.x = -adjustedLocalPos.x;
			//if (parentScale.y < 0) adjustedLocalPos.y = -adjustedLocalPos.y;

			Mtx33Identity(transform.localSpace);
			transform.localSpace.translateThis(transform.Pos()).rotDegThis(transform.Rot()).scaleThis(transform.Scale());

			Matrix3x3<float> parentRotationMatrix;
			Mtx33Identity(parentRotationMatrix);
			parentRotationMatrix.translateThis(parentTranslation).rotDegThis(parentRotation).scaleThis(parentScale);

			transform.worldSpace = parentRotationMatrix * transform.localSpace;
		}
	}

	void TransformSystem::PostUpdate()
	{
		for (std::map<unsigned int, std::unordered_set<unsigned int>>::iterator set{ transformedMap.begin() }; set != transformedMap.end(); ++set)
		{
			for (std::unordered_set<unsigned int>::iterator entity{ set->second.begin()}; entity != set->second.end(); ++entity)
				ComponentManager::GetInstance()->GetComponent<Transform>(*entity).ResetUpdate();
			set->second.clear();
		}
	}

	void TransformSystem::CalculateTransform(Entity entityID, Entity parentID, bool ToWorld)
	{
		Transform& entityTransform = ComponentManager::GetInstance()->GetComponent<Transform>(entityID);
		Transform& parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(parentID);

		// no work --TODO--
		if (ToWorld)
		{
			Mtx3x3f parentInvMtx{};
			float det{};
			Mtx33Inverse(&parentInvMtx, &det, parentTransform.worldSpace);
			// Get back world transform pos
			entityTransform.Pos(entityTransform.Pos().x + parentInvMtx.m20, entityTransform.Pos().y + parentInvMtx.m21);

			parentInvMtx.m20 = parentInvMtx.m21 = 0.f;

			// get back world transform rot
			Vec2f calcRot = entityTransform.worldSpace * Vec2f::right();
			entityTransform.Rot(atan2f(calcRot.y, calcRot.x) / PI * 180.f + entityTransform.Rot());

			// get back world transform scale
			entityTransform.Scale((parentInvMtx.m00 + parentInvMtx.m10) * entityTransform.Scale().x, (parentInvMtx.m11 + parentInvMtx.m01) * entityTransform.Scale().y);
			entityTransform.parent = 0;
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

			CM_CORE_INFO("Entity Position : ({},{})", deltaX, deltaY);

			//entityTransform.scale /= parentTransform.scale; // doesntw ork for some reason
			entityTransform.Scale(entityTransform.Scale().x / parentTransform.Scale().x, entityTransform.Scale().y / parentTransform.Scale().y);

			// calculate the new rotation
			entityTransform.Rot(entityTransform.Rot() - parentTransform.Rot());

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
				
			}
			else if (ComponentManager::GetInstance()->HasComponent<UITransform>(castedMsg->mEntityID))
			{
				UITransform& entityTransform = ComponentManager::GetInstance()->GetComponent<UITransform>(castedMsg->mEntityID);

				entityTransform.parent = castedMsg->mParentID;
			}
		}
	}
}