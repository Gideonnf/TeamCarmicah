#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "ECS/BaseSystem.h"

namespace Carmicah
{
	class GraphicsSystem : public BaseSystem
	{
	private:

	public:
		void Init(const unsigned int&, const unsigned int&);

		void Update();

		void Render();

		void Exit();
	};
}

#endif