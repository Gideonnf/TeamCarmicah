/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TextGraphicsSystem.h

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
	class TextGraphicsSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		const std::string model{ "Square" };
		std::set<unsigned int> mCpyEntitySet;
	public:
		/*!*************************************************************************
		brief
			Sets which components the system references, and load the shader
		***************************************************************************/
		void Init();

		/*!*************************************************************************
		brief
			Clears up the buffer with the id for new fonts to use when it is destroyed
		param[id]
			the id of the font being destroyed
		***************************************************************************/
		void EntityDestroyed(Entity id);

		/*!*************************************************************************
		brief
			Renders text of objects with text and UITransform Components
		***************************************************************************/
		void Update();
	};
}
#endif