#ifndef TEXT_SYSTEM_H
#define TEXT_SYSTEM_H

#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class TextSystem : public BaseSystem
	{
	private:
		const char* shaderName{ "font" };
		GLuint mCurrShader{};

	public:
		void Init();

		void Render(GLuint canvasWidth, GLuint canvasHeight);
	};
}
#endif