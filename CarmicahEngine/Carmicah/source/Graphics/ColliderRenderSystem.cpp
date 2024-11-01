/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ColliderRenderSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Collider Render System handles rendering the collision boxes of gameobjects with the collider2D component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include <ECS/ECSTypes.h>
#include "Graphics/ColliderRenderSystem.h"
#include "Systems/GOFactory.h"
#include "Components/Transform.h"
#include "Components/Collider2D.h"
#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"

namespace Carmicah
{
	void ColliderRenderSystem::Init()
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Collider2D>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<ColliderRenderSystem>(mSignature);
		auto shdrRef = AssetManager::GetInstance()->GetAsset<Shader>("debug");
		mCurrShader = shdrRef.s;
	}

	void ColliderRenderSystem::Render(Entity& cam)
	{
		if (mCurrShader == 0)
			return;
		glUseProgram(mCurrShader);

		for (auto& entity : mEntitiesSet)
		{
			auto& camera = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
			auto& collider = ComponentManager::GetInstance()->GetComponent<Collider2D>(entity);
			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(collider.shape)};


			Matrix3x3<float> trans{};
			trans.translateThis((collider.max.x + collider.min.x) * 0.5f, (collider.max.y + collider.min.y) * 0.5f)
				.scaleThis(collider.max.x - collider.min.x, collider.max.y - collider.min.y);
			trans = camera.camSpace * trans;

			GLint uniformLoc;
			if (UniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, trans.m);

			if (UniformExists(mCurrShader, "uDepth", uniformLoc))
				glUniform1f(uniformLoc, CalcDepth(mNearestDepth, RENDER_LAYERS::UI_LAYER));


			RenderPrimitive(p);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}