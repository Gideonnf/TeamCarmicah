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
            ImGui::Text("Bool: ");
            auto value = GetVariantValueAs<bool>(condition);
            ImGui::SameLine();
            if (ImGui::Checkbox("##BoolCond", &value))
            {
                condition = value;
            }
        }

        if (std::strcmp(varType.c_str(), "int") == 0)
        {
            ImGui::Text("Int: ");
            auto value = GetVariantValueAs<int>(condition);
            ImGui::SameLine();
            if (ImGui::InputInt("##IntCond", &value, 1))
            {
                condition = value;
            }
        }

        if (std::strcmp(varType.c_str(), "float") == 0)
        {
            ImGui::Text("Float: ");
            auto value = GetVariantValueAs<float>(condition);
            ImGui::SameLine();
            if (ImGui::InputFloat("##FloatCond", &value, 1))
            {
                condition = value;
            }
        }

        if (std::strcmp(varType.c_str(), "string") == 0)
        {
            ImGui::Text("String: ");
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

    void FSMWindow::DisplayState(std::string stateName, State& actualState)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        std::string varType = GetVarType(actualState.stateCondition);
        variantVar& var = actualState.stateCondition;

        if (ImGui::TreeNodeEx(stateName.c_str(), flags))
        {
            ImGui::Text("Condition:");
            ImGui::SameLine();
            //ImGui::Text(GetVarType(actualState.stateCondition).c_str());
            VarConditionEditing(varType, actualState.stateCondition);

            for (auto& transition : actualState.transitions)
            {
                bool modified = DisplayTransition(transition, actualState);
                
                if (modified)
                    break;
            }

            ImGui::TreePop();
        }
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
            ImGui::Text(varType.c_str());
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

                    for (auto& [stateName, actualState] : stateMach.stateMap)
                    {
                        DisplayState(stateName, actualState);
                    }
                }
            }



        }
        ImGui::End();
    }

}