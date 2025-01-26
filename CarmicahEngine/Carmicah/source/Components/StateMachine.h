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

	struct Transition
	{
		std::string targetState;
		variantCondition condition;
	};

	struct State
	{
		
		std::string stateName;
		//float stateTime;
		//Script stateScript;
		variantCondition stateCondition;
		std::vector<Transition> transitions;
	};

	struct StateMachine : BaseComponent<StateMachine>
	{
		std::string currState;
		std::string nextState;
		std::string startingState;
		std::unordered_map<std::string, State> stateMap;

		float stateTimer;

		StateMachine& DeserializeComponent(const rapidjson::Value& component) override
		{

			return *this;
		}

		void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
		{

		}
	};
}

#endif