#ifndef ANIMAtiON_SYSTEM_H
#define ANIMAtiON_SYSTEM_H

#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class AnimationSystem : public BaseSystem
	{
	public:
		void Init();

		void Update();
	};
}

#endif