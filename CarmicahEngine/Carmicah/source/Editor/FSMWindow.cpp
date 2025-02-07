/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			FSMWindow.cpp

 author:		Nicholas Lai (100%)
 co-author(s):	

 email:			n.lai@digipen.edu

 brief:			This FSMWindow class is a derived class from EditorWindow.
                It currently provides the implementation of the FSMWindow to manage the FSM.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include "EditorWindow.h"
#include "FSMWindow.h"

namespace Carmicah
{
    FSMWindow::FSMWindow() : EditorWindow("FSM", ImVec2(900, 300), ImVec2(0, 0))
    {
        mIsVisible = true;
    }

    std::string FSMWindow::GetVarType(variantVar& var)
    {
        return std::visit([&](auto&& value) -> std::string
            {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, int>)
                {
                    return "int";
                }
                else if constexpr (std::is_same_v<T, float>)
                {
                    return "float";
                }
                else if constexpr (std::is_same_v<T, bool>)
                {
                    return "bool";
                }
                else if constexpr (std::is_same_v<T, std::string>)
                {
                    return "string";
                }
                else
                    return "error";
            }, var);
    }
    
    void FSMWindow::VarConditionEditing(std::string varType, variantVar& condition)
    {


        if (std::strcmp(varType.c_str(), "bool") == 0)
        {
            ImGui::Text("Bool");
            auto value = GetVariantValueAs<bool>(condition);
            ImGui::SameLine();
            if (ImGui::Checkbox("##BoolCond", &value))
            {
                condition = value;
            }
        }

        if (std::strcmp(varType.c_str(), "int") == 0)
        {
            ImGui::Text("Int");
            auto value = GetVariantValueAs<int>(condition);
            ImGui::SameLine();
            if (ImGui::InputInt("##IntCond", &value, 1))
            {
                condition = value;
            }
        }

        if (std::strcmp(varType.c_str(), "float") == 0)
        {
            ImGui::Text("Float");
            auto value = GetVariantValueAs<float>(condition);
            ImGui::SameLine();
            if (ImGui::InputFloat("##FloatCond", &value, 1))
            {
               condition = value;
            }
        }

        if (std::strcmp(varType.c_str(), "string") == 0)
        {
            ImGui::Text("String");
            auto value = GetVariantValueAs<std::string>(condition);
            static char buffer[256];
            std::strncpy(buffer, value.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';
            ImGui::SameLine();
            if (ImGui::InputText("##StringCond", buffer, sizeof(buffer)))
            {
                condition = std::string(buffer);
            }
        }
    }

    void FSMWindow::CreateNewState(StateMachine& component)
    {
        auto& stateMap = component.stateMap;

        if (ImGui::Button("Create New State"))
        {
            ImGui::OpenPopup("CreateNewState");
        }

        if (ImGui::BeginPopup("CreateNewState"))
        {
            static State newState;
            static char buffer[128]{ "Default\0" };
            ImGui::Text("State Name: ");
            ImGui::SameLine();
            if (ImGui::InputText("##State Name:", buffer, sizeof(buffer)))
            {
                newState.stateName = buffer;
            }
            //static std::string varType{};
            //const char* items[] = { "", "bool", "int", "float", "string" };
            //static int currentItem = 0; // Index of selected item
            //static variantVar condition;
            //static bool boolValue = false;
            //static int intValue = 0;
            //static float floatValue = 0.f;
            //static char stringValue[256] = "Default";

            //if (ImGui::Combo("##Select Type", &currentItem, items, IM_ARRAYSIZE(items)))
            //{
            //    varType = items[currentItem];
            //}
            //ImGui::Text("Condition Type:");
            //ImGui::SameLine();

            //if (std::strcmp(varType.c_str(), "bool") == 0)
            //{
            //    ImGui::Text("Bool: ");
            //    ImGui::SameLine();
            //    if (ImGui::Checkbox("##BoolCond", &boolValue))
            //    {
            //        condition = boolValue;
            //    }
            //}

            //if (std::strcmp(varType.c_str(), "int") == 0)
            //{
            //    ImGui::Text("Int: ");
            //    ImGui::SameLine();
            //    if (ImGui::InputInt("##IntCond", &intValue, 1))
            //    {
            //        condition = intValue;
            //    }
            //}

            //if (std::strcmp(varType.c_str(), "float") == 0)
            //{
            //    ImGui::Text("Float: ");
            //    ImGui::SameLine();
            //    if (ImGui::InputFloat("##FloatCond", &floatValue, 1))
            //    {
            //        condition = floatValue;
            //    }
            //}

            //if (std::strcmp(varType.c_str(), "string") == 0)
            //{
            //    ImGui::Text("String: ");
            //    ImGui::SameLine();
            //    if (ImGui::InputText("##StringCond", stringValue, sizeof(stringValue) - 1))
            //    {
            //        condition = std::string(stringValue);
            //    }
            //}

            /*if (currentItem != 0)
            {*/
                if (ImGui::Button("Create State"))
                {
                    newState.stateName = buffer;
                    auto it = stateMap.begin();
                    for (; it != stateMap.end(); ++it)
                    {
                        if (it->second == newState)
                        {
                            CM_CORE_WARN("Trying to create a state that already exists");
                            break;
                        }

                        if (it->first == newState.stateName)
                        {
                            CM_CORE_WARN("Trying to create a stateName that already exists");
                            break;
                        }
                        
                    }
                    
                    if (it != stateMap.end())
                    {
                        CM_CORE_WARN("Unable to create State, check other warnings and try again");
                    }
                    else
                    {
                        stateMap[newState.stateName] = newState;

                        //Resetting the static variables
                        strncpy(buffer, "Default", sizeof(buffer) - 1);
                        buffer[sizeof(buffer) - 1] = '\0';
                        /*currentItem = 0;
                        boolValue = false;
                        intValue = 0;
                        floatValue = 0.f;
                        strncpy(stringValue, "Default", sizeof(stringValue) - 1);
                        stringValue[sizeof(stringValue) - 1] = '\0';*/
                        ImGui::CloseCurrentPopup();
                    }
                }
            //}

            ImGui::EndPopup();
        }
    }

    void FSMWindow::AddTransition(State& actualState, StateMachine& stateMach)
    {
        auto& stateMap = stateMach.stateMap;
        std::vector<const char*> stateNames = {""};
        static int selectedState = 0;


        for (const auto& state : stateMap)
        {
            bool addToList = true;
            if (state.first == actualState.stateName)
            {
                addToList = false;
            }

            for (auto& it : actualState.transitions)
            {
                if (state.first == it.targetState)
                {
                    addToList = false;
                }
            }
            if(addToList)
            {
                stateNames.push_back(state.first.c_str());
            }
        }

        if (ImGui::Button("Add Transition"))
        {
            ImGui::OpenPopup("New Transition");
        }

        if (ImGui::BeginPopup("New Transition"))
        {
            static Transition newTransition;
            static std::string varType{};
            
            const char* items[] = { "", "bool", "int", "float", "string" };
            static char buffer[128]{ "Default\0" };
            static variantVar condition;
            static bool boolValue = false;
            static int intValue = 0;
            static float floatValue = 0.f;
            static char stringValue[256] = "Default";
            ImGui::Text("Target State: ");
            ImGui::SameLine();

            if (ImGui::Combo("##StateNames", &selectedState, stateNames.data(), (int)stateNames.size()))
            {
                std::strncpy(buffer, stateNames[selectedState], sizeof(buffer));
            }


            static int currentItem = 0; // Index of selected item

            if (ImGui::Combo("##Select Type", &currentItem, items, IM_ARRAYSIZE(items)))
            {
                varType = items[currentItem];
            }
            ImGui::Text("Condition Type: %s", varType.c_str());

            if (std::strcmp(varType.c_str(), "bool") == 0)
            {
                ImGui::Text("Bool: ");
                ImGui::SameLine();
                if (ImGui::Checkbox("##BoolCond", &boolValue))
                {
                    condition = boolValue;
                }
            }

            if (std::strcmp(varType.c_str(), "int") == 0)
            {
                ImGui::Text("Int: ");
                ImGui::SameLine();
                if (ImGui::InputInt("##IntCond", &intValue, 1))
                {
                    condition = intValue;
                }
            }

            if (std::strcmp(varType.c_str(), "float") == 0)
            {
                ImGui::Text("Float: ");
                ImGui::SameLine();
                if (ImGui::InputFloat("##FloatCond", &floatValue, 1))
                {
                    condition = floatValue;
                }
            }

            if (std::strcmp(varType.c_str(), "string") == 0)
            {
                ImGui::Text("String: ");
                ImGui::SameLine();
                if (ImGui::InputText("##StringCond", stringValue, sizeof(stringValue) - 1))
                {
                    condition = std::string(stringValue);
                }
            }

            if (currentItem != 0 && selectedState != 0)
            {
                if (ImGui::Button("Create Transition"))
                {
                    newTransition.targetState = buffer;
                    newTransition.condition = condition;
                    auto it = std::find(actualState.transitions.begin(), actualState.transitions.end(), newTransition);
                    auto sameTargetState = std::find_if(actualState.transitions.begin(), actualState.transitions.end(), [&](Transition& curr)
                        {
                            return curr.targetState == newTransition.targetState;
                        });
                    auto sameCond = std::find_if(actualState.transitions.begin(), actualState.transitions.end(), [&](Transition& curr)
                        {
                            return curr.condition == newTransition.condition;
                        });
                    if (it != actualState.transitions.end())
                    {
                        CM_CORE_WARN("Creating an already existing transition!");
                    }
                    else if (sameTargetState != actualState.transitions.end())
                    {
                        CM_CORE_WARN("A condition already exists for that targetState!");
                    }
                    else if (sameCond != actualState.transitions.end())
                    {
                        CM_CORE_WARN("Two targetStates with the same condition!");
                    }
                    else
                    {
                        actualState.transitions.push_back(newTransition);

                        //Resetting the static variables
                        strncpy(buffer, "Default", sizeof(buffer) - 1);
                        buffer[sizeof(buffer) - 1] = '\0';
                        selectedState = 0;

                        varType = {};
                        currentItem = 0;
                        boolValue = false;
                        intValue = 0;
                        floatValue = 0.f;
                        strncpy(stringValue, "Default", sizeof(stringValue) - 1);
                        stringValue[sizeof(stringValue) - 1] = '\0';
                        ImGui::CloseCurrentPopup();
                    }
                }
            }


            ImGui::EndPopup();
        }
    }

    bool FSMWindow::DisplayState(std::string stateName, State& actualState, StateMachine& stateMach)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanTextWidth;
        std::string varType = GetVarType(actualState.stateCondition);
        variantVar& var = actualState.stateCondition;

        UNUSED(var);

        if (ImGui::TreeNodeEx(stateName.c_str(), flags))
        {
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup(stateName.c_str());
            }


            ImGui::SameLine();
            AddTransition(actualState, stateMach);

            //ImGui::Text("Condition:");
            //ImGui::SameLine();
            //ImGui::Text(GetVarType(actualState.stateCondition).c_str());
            //VarConditionEditing(varType, actualState.stateCondition);

            if (ImGui::BeginPopup(stateName.c_str()))
            {
                if (ImGui::Selectable("Delete State"))
                {
                    stateMach.DeleteState(actualState);
                    ImGui::EndPopup();
                    ImGui::TreePop();
                    return true;
                }
                ImGui::EndPopup();
            }

            ImGui::Text("List of Transitions:");

            for (auto& transition : actualState.transitions)
            {
                bool modified = DisplayTransition(transition, actualState);
                
                if (modified)
                    break;
            }

            ImGui::TreePop();
        }
        return false;
    }

    bool FSMWindow::DisplayTransition(Transition& currentTransition, State& currentState)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (ImGui::TreeNodeEx(currentTransition.targetState.c_str(), flags))
        {
            if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup(currentTransition.targetState.c_str());
            }


            ImGui::Text("Condition Type:");
            ImGui::SameLine();
            std::string varType = GetVarType(currentTransition.condition);
            VarConditionEditing(varType, currentTransition.condition);

            if (ImGui::BeginPopup(currentTransition.targetState.c_str()))
            {
                if (ImGui::Selectable("Delete Transition"))
                {
                    currentState.DeleteTransition(currentTransition);
                    ImGui::EndPopup();
                    ImGui::TreePop();
                    return true;
                }

                ImGui::EndPopup();
            }

            ImGui::TreePop();
        }
        return false;
    }

    void FSMWindow::Update()
    {
        if (ImGui::Begin(mTitle))
        {
            if (HierarchyWindow::selectedGO != nullptr)
            {
                if (HierarchyWindow::selectedGO->HasComponent<StateMachine>())
                {
                    StateMachine& stateMach = HierarchyWindow::selectedGO->GetComponent<StateMachine>();

                    CreateNewState(stateMach);

                    for (auto& [stateName, actualState] : stateMach.stateMap)
                    {
                        bool isModified = DisplayState(stateName, actualState, stateMach);
                        ImGui::Dummy(ImVec2(10.0, 20.0));

                        if (isModified)
                            break;
                    }
                }
            }
            else if (HierarchyWindow::inspectedPrefab != nullptr)
            {
                if (HierarchyWindow::inspectedPrefab->HasComponent<StateMachine>())
                {
                    StateMachine& stateMach = HierarchyWindow::inspectedPrefab->GetComponent<StateMachine>();

                    CreateNewState(stateMach);

                    for (auto& [stateName, actualState] : stateMach.stateMap)
                    {
                        bool isModified = DisplayState(stateName, actualState, stateMach);
                        ImGui::Dummy(ImVec2(10.0, 20.0f));

                        if (isModified)
                            break;
                    }
                }
            }
        }
        ImGui::End();
    }

}