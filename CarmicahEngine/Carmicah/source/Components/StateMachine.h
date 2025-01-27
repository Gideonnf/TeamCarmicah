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

		// If it links with any animation can add its info here
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
			// if this works this will be q cool
			DESERIALIZE_IF_HAVE(currState, component, "currState", GetString, std::string);
			DESERIALIZE_IF_HAVE(nextState, component, "nextState", GetString, std::string);
			DESERIALIZE_IF_HAVE(nextState, component, "startingState", GetString, std::string);

			return *this;
		}

		void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
		{

		}
	};
}

#endif