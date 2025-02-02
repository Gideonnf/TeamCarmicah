/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			FSMWindow.h

 author:		Nicholas Lai (100%)
 co-author(s):

 email:			n.lai@digipen.edu

 brief:			This file implements the FSMWindow class, which provides an interface for managing the states and the
 state machine in a GameObject. This allows the user to edit the states, transitions and other important conditions regarding
 the FSM.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef FSMWINDOW_H_
#define FSMWINDOW_H_

#include <GLFW/glfw3.h>
#include <ImGUI/imgui.h>
#include "EditorWindow.h"

namespace Carmicah
{
    class FSMWindow : public EditorWindow
    {
    private:
   

    public:
        FSMWindow();
        void Update() override;
    };
}


#endif