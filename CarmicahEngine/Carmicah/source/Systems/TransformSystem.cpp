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

	void TransformSystem::ReceiveMessage(Message* msg)
	{
	}
}