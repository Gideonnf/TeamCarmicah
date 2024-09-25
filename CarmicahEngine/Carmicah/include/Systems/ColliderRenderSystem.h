#ifndef COLLIDER_RENDERER_SYSTEM_H
#define COLLIDER_RENDERER_SYSTEM_H

#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class ColliderRenderSystem : public BaseSystem
	{
	private:
		const char* shaderName{ "debug" };
		GLuint currShader{};
	public:
		void Init();

		void Render(Entity& cam);
	};
}

#endif