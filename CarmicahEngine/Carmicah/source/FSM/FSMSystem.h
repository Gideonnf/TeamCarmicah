/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			FSMSystem.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			Handles the calling and looping of state machine components for all entities.
				Will call the respective script functions to trigger any script behaviour

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


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

		/// <summary>
		/// Initialises the state machines by setting their initial next state
		/// </summary>
		void Init();
		/// <summary>
		/// Loops through all the entities with a state machine. Call on Update aend check for transitions
		/// </summary>
		/// <param name="dt"></param>
		void OnUpdate(float dt);

		/// <summary>
		/// Entering a new state will call the appropriate script function
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="state"></param>
		void EnterState(Entity entity, State state);
		/// <summary>
		/// Updating a state will call the appropriate script function
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="state"></param>
		/// <param name="dt"></param>
		void UpdateState(Entity entity, State state, float dt);
		/// <summary>
		/// Exiting a state will call the appropriate exit function from the old state
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="state"></param>
		void ExitState(Entity entity, State state);

		/// <summary>
		/// In order to set the condition of a variable in the entity
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="var"></param>
		void SetCondition(Entity entity, variantVar var);
		/// <summary>
		/// Set state not being used atm
		/// </summary>
		/// <param name="entity"></param>
		/// <param name="stateName"></param>
		void SetState(Entity entity, std::string stateName);

		/// <summary>
		/// Returns the timer of the current state
		/// </summary>
		/// <param name="entity"></param>
		/// <returns></returns>
		float GetStateTimer(Entity entity);
	};
}


#endif
