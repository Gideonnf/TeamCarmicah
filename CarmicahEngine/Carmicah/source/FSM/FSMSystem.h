#ifndef FSM_SYSTEM_H
#define FSM_SYSTEM_H

#include "../ECS/BaseSystem.h"

namespace Carmicah
{
	class FSMSystem : public BaseSystem
	{
	public:
		FSMSystem();
		~FSMSystem();

		void Init();
		void OnUpdate(float dt);


	};
}


#endif
