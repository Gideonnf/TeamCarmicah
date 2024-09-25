#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class GraphicsSystem : public BaseSystem
	{
	private:
		const char* shaderName{"basic"};
		GLuint currShader;
	public:
		void Init();

		void Render(Entity& cam);
	};
}

#endif