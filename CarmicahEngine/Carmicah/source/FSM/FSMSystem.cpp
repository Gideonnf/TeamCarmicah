#include "pch.h"
#include "FSMSystem.h"
#include "../Components/StateMachine.h"
#include "../ECS/ComponentManager.h"
#include "../ECS/EntityManager.h"
#include "../ECS/SystemManager.h"

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

			// if there is a difference in the curr state vs next state
			if (stateMachine.currState != stateMachine.nextState)
			{
				// if the curr state isnt empty means its exiting a previous state
				if (stateMachine.currState.empty() == false)
				{
					// call the onExit function for the curr state
				}

				// call the onEnter function for next state


				stateMachine.currState = stateMachine.nextState;
				// reset the state timer when entering a new state
				stateMachine.stateTimer = 0.0f;
			}
			// if its the same then call the on update
			else if (stateMachine.currState == stateMachine.nextState)
			{
				// call on upate function

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
}