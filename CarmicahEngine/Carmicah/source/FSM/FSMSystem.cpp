/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			FSMSystem.cpp

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			Handles the calling and looping of state machine components for all entities.
				Will call the respective script functions to trigger any script behaviour

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


#include "pch.h"
#include "FSMSystem.h"
#include "../Components/StateMachine.h"
#include "../ECS/ComponentManager.h"
#include "../ECS/EntityManager.h"
#include "../ECS/SystemManager.h"
#include "../Messaging/Message.h"
namespace Carmicah
{
	FSMSystem::FSMSystem()
	{
		mSignature.set(ComponentManager::GetInstance()->GetComponentID<StateMachine>());

		SystemManager::GetInstance()->SetSignature<FSMSystem>(mSignature);


	}

	FSMSystem::~FSMSystem()
	{

	}

	void FSMSystem::Init()
	{
		for (auto it = mEntitiesSet.begin(); it != mEntitiesSet.end(); ++it)
		{
			auto* componentManager = ComponentManager::GetInstance();
			auto& stateMachine = componentManager->GetComponent<StateMachine>(*it);
			if (stateMachine.stateMap.size() == 0) continue;// no states to run

			if (stateMachine.startingState.empty())
			{
				stateMachine.startingState = stateMachine.stateMap.begin()->first;
			}

			// initialize the next state to be the starting state
			stateMachine.nextState = stateMachine.startingState;
		}
	}

	void FSMSystem::OnUpdate(float dt)
	{
		for (auto it = mEntitiesSet.begin(); it != mEntitiesSet.end(); ++it)
		{
			auto* componentManager = ComponentManager::GetInstance();
			auto& stateMachine = componentManager->GetComponent<StateMachine>(*it);
			if (stateMachine.stateMap.size() == 0) continue;// no states to run

			stateMachine.stateTimer += dt;

			// if its a runtime obj thats added and starting state isnt initalized properly
			if (stateMachine.startingState.empty())
			{
				stateMachine.startingState = stateMachine.stateMap.begin()->first;
				// initialize the next state to be the starting state
				stateMachine.nextState = stateMachine.startingState;
			}

			// if there is a difference in the curr state vs next state
			if (stateMachine.currState != stateMachine.nextState)
			{
				// if the curr state isnt empty means its exiting a previous state
				if (stateMachine.currState.empty() == false)
				{
					// call the onExit function for the curr state
					ExitState(*it, stateMachine.stateMap[stateMachine.currState]);
					// reset state condition
					stateMachine.stateMap[stateMachine.currState].stateCondition = 0;
				}

				// call the onEnter function for next state
				EnterState(*it, stateMachine.stateMap[stateMachine.nextState]);


				stateMachine.currState = stateMachine.nextState;
				// reset the state timer when entering a new state
				stateMachine.stateTimer = 0.0f;
			}
			// if its the same then call the on update
			else if (stateMachine.currState == stateMachine.nextState)
			{
				// call on upate function
				UpdateState(*it, stateMachine.stateMap[stateMachine.currState], dt);
			}

			// check the transitions for this state
			for (const auto& transition : stateMachine.stateMap[stateMachine.currState].transitions)
			{
				// if the condition is met
				if (stateMachine.stateMap[stateMachine.currState].stateCondition == transition.condition)
				{
					stateMachine.nextState = transition.targetState;
					break;
				}
			}
		}
	}

	// But only using it with scripts for now so use this to call the script
	// but now idk if i should make an entire new script map system for state scripts
	// or just add in the function call to normal entity scripts

	void FSMSystem::EnterState(Entity entity, State state)
	{
		// if it has an animation can trigger animation play here
		// if it has an sound can trigger sound play here

		OnStateEnterMsg newMsg(entity, state.stateName);
		SendSysMessage(&newMsg);
	}

	void FSMSystem::UpdateState(Entity entity, State state, float dt)
	{
		// if it has an animation can trigger animation play here
		// if it has an sound can trigger sound play here

		OnStateUpdateMsg newMsg(entity, state.stateName, dt);
		SendSysMessage(&newMsg);
	}

	void FSMSystem::ExitState(Entity entity, State state)
	{
		// if it has an animation can trigger animation play here
		// if it has an sound can trigger sound play here


		OnStateExitMsg newMsg(entity, state.stateName);
		SendSysMessage(&newMsg);
	}

	void FSMSystem::SetCondition(Entity entity, variantVar var)
	{
		for (auto it = mEntitiesSet.begin(); it != mEntitiesSet.end(); ++it)
		{
			auto* componentManager = ComponentManager::GetInstance();
			auto& stateMachine = componentManager->GetComponent<StateMachine>(*it);
			if (stateMachine.stateMap.size() == 0) continue;

			// if it found the entity
			if (*it == entity)
			{
				stateMachine.stateMap[stateMachine.currState].stateCondition = var;
				break;
			}
		}
	}

	void FSMSystem::SetState(Entity entity, std::string stateName)
	{
		UNUSED(entity);
		UNUSED(stateName);
	}

	float FSMSystem::GetStateTimer(Entity entity)
	{
		for (auto it = mEntitiesSet.begin(); it != mEntitiesSet.end(); ++it)
		{
			auto* componentManager = ComponentManager::GetInstance();
			auto& stateMachine = componentManager->GetComponent<StateMachine>(*it);
			if (stateMachine.stateMap.size() == 0) continue;

			// if it found the entity
			if (*it == entity)
			{
				return stateMachine.stateTimer;
			}
		}
		return 0.0f;
	}
}