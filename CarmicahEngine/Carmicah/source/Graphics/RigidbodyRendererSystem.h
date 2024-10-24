/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RigidbodyRendererSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Rigidbody Render System handles rendering the rigidbody parts such as velocity of gameobjects with the rigidbody component

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RIGIDBODY_RENDERER_SYSTEM_H
#define RIGIDBODY_RENDERER_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "Graphics/BaseGraphicsSystem.h"

namespace Carmicah
{
	class RigidbodyRendererSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		const char* shaderName{ "debug" };
		const char* modelName{ "DebugLine" };
		GLuint mCurrShader{};
	public:
		/*
		brief
			Sets which components the system references, and load the shader
		*/
		void Init();

		/*
		brief
			Renders the Debug line of entities
		param[cam]
			camera dimensions and position to reference
		*/
		void Render(Entity& cam);
	};
}

#endif