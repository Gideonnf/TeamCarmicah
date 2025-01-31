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

		void UpdateState(State state);
		void EnterState(State state);
		void ExitState(State state);
	};
}


#endif
