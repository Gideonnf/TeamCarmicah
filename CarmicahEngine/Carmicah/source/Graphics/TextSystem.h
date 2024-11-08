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
#include "Graphics/BaseGraphicsSystem.h"

namespace Carmicah
{
	class TextSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		Mtx3x3f screenMtx{};
		int mLastTextNumCount{};

	public:
		/*
		brief
			Sets which components the system references, and load the shader
		param[canvasWidth]
			width of reference canvas
		param[canvasHeight]
			height of reference canvas
		*/
		void Init(const float& canvasWidth, const float& canvasHeight);

		/*
		brief
			Renders text of objects with text and UITransform Components
		*/
		void Render();
	};
}
#endif