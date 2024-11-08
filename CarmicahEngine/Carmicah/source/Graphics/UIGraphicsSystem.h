/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			UIGraphicsSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			UI Graphics System handles the rendering of textured meshes onto the screen (using transform, screen reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef UI_GRAPHICS_SYSTEM_H
#define UI_GRAPHICS_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "Graphics/BaseGraphicsSystem.h"
#include "Math/Matrix3x3.h"

namespace Carmicah
{
	class UIGraphicsSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		Mtx3x3f screenMtx{};	// Calculated Screen Matrix multiplier to use for text systems

	public:
		/*!*************************************************************************
		brief
			Sets which components the system references, loads the shader, and calculates the screenMultiplcationMatrix
		param[screenWidth]
			Screen dimensions (width)
		param[screenHeight]
			Screen dimensions (height)
		***************************************************************************/
		void Init(const float& screenWidth, const float& screenHeight);

		/*!*************************************************************************
		brief
			Renders the objects based on screen position [0,0] -> (top left)
		***************************************************************************/
		void Render();

	};
}

#endif