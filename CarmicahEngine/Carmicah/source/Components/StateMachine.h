#ifndef STATE_H
#define STATE_H

#include "BaseComponent.h"
#include <string>
#include <unordered_map>
#include "Script.h"

namespace Carmicah
{
	struct Transition
	{
		std::string targetState;
		variantVar condition;

		bool operator==(const Transition& other) const
		{
			return targetState == other.targetState && condition == other.condition;
		}
	};

	struct State
	{
		//float stateTime;
		//Script stateScript;
		
		std::string stateName;
		variantVar stateCondition;
		std::vector<Transition> transitions;

		bool operator==(const State& other) const
		{
			bool retVal = true;

			if (this->stateName != other.stateName)
			{
				retVal = false;
			}

			if (this->stateCondition != other.stateCondition)
			{
				retVal = false;
			}

			if (this->transitions != other.transitions)
			{
				retVal = false;
			}

			return retVal;
		}

		void DeleteTransition(Transition& selectedTransition)
		{
			auto it = std::remove_if(transitions.begin(), transitions.end(), [&](const Transition& t)
				{
					return t.condition == selectedTransition.condition;
				});
			if (it != transitions.end())
			{
				transitions.erase(it, transitions.end());
			}
		}

		// If it links with any animation can add its info here
	};

	struct StateMachine : BaseComponent<StateMachine>
	{
		std::string currState;
		std::string nextState;
		std::string startingState;
		std::unordered_map<std::string, State> stateMap;

		float stateTimer;



		void DeleteState(State& selectedState)
		{
			std::string stateName = selectedState.stateName;

			for (auto& it = stateMap.begin(); it != stateMap.end(); ++it)
			{
				if (it->second == selectedState)
				{
					it = stateMap.erase(it);
					break;
				}
			}

			for (auto& it = stateMap.begin(); it != stateMap.end(); ++it)
			{
				for (auto& transIt = it->second.transitions.begin(); transIt != it->second.transitions.end(); ++transIt)
				{
					if (transIt->targetState == stateName)
					{
						it->second.DeleteTransition(*transIt);
						break;
					}
				}
			}
		}

		StateMachine& DeserializeComponent(const rapidjson::Value& component) override
		{
			// if this works this will be q cool
			DESERIALIZE_IF_HAVE(currState, component, "currState", GetString, std::string);
			DESERIALIZE_IF_HAVE(nextState, component, "nextState", GetString, std::string);
			DESERIALIZE_IF_HAVE(nextState, component, "startingState", GetString, std::string);
			//DESERIALIZE_IF_HAVE(nextState, component, "startingState", GetString, std::string);
			if (component.HasMember("stateMap"))
			{
				const rapidjson::Value& stateList = component["stateMap"];
				for (const auto& stateObj : stateList.GetArray())
				{
					State newState;
					newState.stateName = stateObj["stateName"].GetString();
					newState.stateCondition = ReadVariant("stateCondition", stateObj); //stateObj["stateCondition"].GetInt(); //TODO: Make a function to get variant vars
					if (stateObj.HasMember("transitions") && stateObj["transitions"].IsArray())
					{
						const auto& transitions = stateObj["transitions"].GetArray();
						for (const auto& transitionObj : transitions)
						{
							Transition transition;
							transition.targetState = transitionObj["targetState"].GetString();
							transition.condition = ReadVariant("condition", transitionObj);//transitionObj["condition"].GetInt(); // TODO: Make a unction to get variant var
						
							newState.transitions.push_back(transition);
						}
					}

					stateMap[newState.stateName] = newState;
				}
			}
			return *this;
		}

		void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
		{
			writer.String("currState");
			writer.String(currState.c_str());
			writer.String("nextState");
			writer.String(nextState.c_str());
			writer.String("startingState");
			writer.String(startingState.c_str());
			writer.String("stateMap");
			writer.StartArray();
			for (const auto& [name, state] : stateMap)
			{
				writer.StartObject();
				writer.String("stateName");
				writer.String(name.c_str());

				writer.String("stateCondition");
				WriteVariant(state.stateCondition, writer);

				writer.String("transitions");
				writer.StartArray();
				for (const auto& transition : state.transitions)
				{
					writer.StartObject();
					writer.String("targetState");
					writer.String(transition.targetState.c_str());
					writer.String("condition");
					WriteVariant(transition.condition, writer);
					writer.EndObject();
				}
				writer.EndArray();

				writer.EndObject();
			}
			writer.EndArray();
		}
	};
}

#endif