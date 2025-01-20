/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RenderTransformSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Transform System. Header file that contains the function declarations of the Render Transform System.
				The system only handles changing the internal scale calculations for Images when texture changes

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef RENDER_TRANSFORM_SYSTEM_H
#define RENDER_TRANSFORM_SYSTEM_H

#include "../ECS/GameObject.h"
#include "../ECS/BaseSystem.h"

namespace Carmicah
{
	class RenderTransformSystem : public BaseSystem
	{
	public:
		/// <summary>
		/// Initializes the transform system by registering the components
		/// </summary>
		void Init();
		/// <summary>
		/// If any entities are updated, then recalculate their local and world space matrix transform used in graphics
		/// </summary>
		void Update();
	};

}

#endif