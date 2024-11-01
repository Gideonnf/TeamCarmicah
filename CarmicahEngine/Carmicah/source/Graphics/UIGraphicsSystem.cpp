/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			UIGraphicsSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			UI Graphics System handles the rendering of textured meshes onto the screen (using transform, screen reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "Graphics/UIGraphicsSystem.h"

#include <ECS/ECSTypes.h>
#include "Systems/GOFactory.h"

#include "Components/Transform.h"
#include "Components/Renderer.h"

#include "ECS/SystemManager.h"
#include "ECS/ComponentManager.h"

#include "Systems/AssetManager.h"
#include "log.h"

namespace Carmicah
{
	void UIGraphicsSystem::Init(const float& screenWidth, const float& screenHeight)
	{
		// Set the signature of the system
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<UITransform>());
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<Renderer>());
		// Update the signature of the system
		SystemManager::GetInstance()->SetSignature<UIGraphicsSystem>(mSignature);
		auto& shdrRef = AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.defaultShader);
		mCurrShader = shdrRef.s;
		screenMtx.scaleThis(1 / screenWidth, 1 / screenHeight);
	}

	void UIGraphicsSystem::Render()
	{
		glUseProgram(mCurrShader);
		if (mCurrShader == 0)
			return;

		// Set Uniforms
		GLint uniformLoc{};
		if (UniformExists(mCurrShader, "uModel_to_NDC", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, screenMtx.m);


		for (auto& entity : mEntitiesSet)
		{
			auto& transform = ComponentManager::GetInstance()->GetComponent<Transform>(entity);

			// Get Components
			Renderer& renderer = ComponentManager::GetInstance()->GetComponent<Renderer>(entity);
			auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>(renderer.model) };

			//if (UniformExists(mCurrShader, "uTex2d", uniformLoc)) // Only if multiple textures
			//	glUniform1i(uniformLoc, 0);

			if (UniformExists(mCurrShader, "uDepth", uniformLoc))
				glUniform1f(uniformLoc, CalcDepth(transform.depth));

			if (UniformExists(mCurrShader, "uID", uniformLoc))
				glUniform1ui(uniformLoc, entity);

			if (UniformExists(mCurrShader, "uAnimationMult", uniformLoc))
				glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, renderer.textureMat.m);

			// Error Checking if texture no exists
			auto& tryTex = AssetManager::GetInstance()->GetAsset<Texture>(renderer.texture);
			glBindTextureUnit(0, tryTex.t);

			RenderPrimitive(p);
		}

		glBindTextureUnit(0, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

}