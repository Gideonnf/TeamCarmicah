#ifndef FSM_SYSTEM_H
#define FSM_SYSTEM_H

#include "../ECS/BaseSystem.h"
#include "../Components/StateMachine.h"

namespace Carmicah
{
	class FSMSystem : public BaseSystem
	{
	public:
		FSMSystem();
		~FSMSystem();

		void Init();
		void OnUpdate(float dt);

		void EnterState(Entity entity, State state);
		void UpdateState(Entity entity, State state, float dt);
		void ExitState(Entity entity, State state);
	};
}


#endif
