#ifndef STATE_H
#define STATE_H

#include "BaseComponent.h"
#include <string>
#include <unordered_map>
#include "Script.h"

namespace Carmicah
{
	//enum class 
	using variantCondition = std::variant<int, float, bool>;

	struct State
	{
		
		std::string stateName;
		//float stateTime;
		//Script stateScript;
		variantCondition stateCondition;
		// keep track of which states are connected 
	};

	struct StateMachine : BaseComponent<StateMachine>
	{
		std::string currState;
		std::string nextState;
		std::string startingState;
		std::unordered_map<std::string, State> stateMap;
		std::unordered_map<std::string, std::string> stateTransitions;
		std::unordered_map<std::string, variantCondition> transitionCondition;
		float stateTimer;
	};
}

#endif