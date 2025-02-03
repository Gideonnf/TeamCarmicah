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

		// TODO: Make a map similar to script system
		// so that I can have direct access to entities FSM
		// instead of looping through mEntities
		// but after M4

		void Init();
		void OnUpdate(float dt);

		void EnterState(Entity entity, State state);
		void UpdateState(Entity entity, State state, float dt);
		void ExitState(Entity entity, State state);

		void SetCondition(Entity entity, variantVar var);
		void SetState(Entity entity, std::string stateName);
		float GetStateTimer(Entity entity);
	};
}


#endif
