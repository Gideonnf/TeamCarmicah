#ifndef STATE_H
#define STATE_H

#include "BaseComponent.h"
#include <string>
#include <unordered_map>
#include "Script.h"

namespace Carmicah
{
	struct State
	{
		std::string stateName;
		Script stateScript;
		std::unordered_map<std::string, std::string> stateTransitions;
	};

	struct StateMachine : BaseComponent<StateMachine>
	{
		std::string currState;
		std::unordered_map<std::string, State> stateTransitions;
		float stateTime;
	};
}

#endif