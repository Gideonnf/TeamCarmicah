#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class GraphicsSystem : public BaseSystem
	{
	private:
		const char* shaderName{"basic"};
		GLuint currShader{};

		bool uniformExists(const char* str, GLint& ref);
	public:
		void Init();
		void SetScreenSize(GLuint camWidth, GLuint camHeight, Entity& cam);

		void Render(Entity& cam);

	};
}

#endif