/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TextSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			TextSystem handles the rendering of text to the screen

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
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