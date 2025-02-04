/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:          SceneToImgui.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:         A bridge to map the graphics rendering to ImGui. Binds rendering to a texture that can be mapped to an image in ImGui

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include <sstream>
#include "SceneToImgui.h"
#include "log.h"
#include "Input/InputSystem.h"
#include "ECS/ECSTypes.h"
#include "Graphics/RenderHelper.h"

namespace Carmicah
{
    void SceneToImgui::CreateFramebuffer(int width, int height)
    {
        for (int i{}; i < MAX_SCENES; ++i)
        {
            glGenFramebuffers(1, &mScenes[i].FBO);
            glBindFramebuffer(GL_FRAMEBUFFER, mScenes[i].FBO);

            glCreateTextures(GL_TEXTURE_2D, 1, &mScenes[i].texture_id);
            glTextureStorage2D(mScenes[i].texture_id, 1, GL_RGBA8, width, height);
            glTextureParameterf(mScenes[i].texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameterf(mScenes[i].texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mScenes[i].texture_id, 0);

            glGenTextures(1, &mScenes[i].picker_id);
            glBindTexture(GL_TEXTURE_2D, mScenes[i].picker_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
            glTextureParameterf(mScenes[i].picker_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameterf(mScenes[i].picker_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, mScenes[i].picker_id, 0);

            glGenRenderbuffers(1, &mScenes[i].RBO);
            glBindRenderbuffer(GL_RENDERBUFFER, mScenes[i].RBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mScenes[i].RBO);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
#ifndef CM_RELEASE
                CM_CORE_ERROR(std::string{ "ERROR::FRAMEBUFFER:: Framebuffer(" } + std::to_string(i) + ") is not complete!\n");
#endif
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void SceneToImgui::UnloadFramebuffer()
    {
        for (int i{}; i < MAX_SCENES; ++i)
        {
            glDeleteRenderbuffers(1, &mScenes[i].RBO);
            glDeleteTextures(1, &mScenes[i].picker_id);
            glDeleteTextures(1, &mScenes[i].texture_id);
            glDeleteFramebuffers(1, &mScenes[i].FBO);
        }
    }

    void SceneToImgui::BindFramebuffer(SCENE_IMGUI scene)
    {
        if (scene == NO_SCENE)
        {
            UnbindFramebuffer();
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, mScenes[scene].FBO);
        unsigned int drawBuffers[] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1
        };
        glDrawBuffers(sizeof(drawBuffers) / sizeof(unsigned int), drawBuffers);
    }

    void SceneToImgui::UnbindFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint SceneToImgui::GetTexture(SCENE_IMGUI scene)
    {
        return mScenes[scene].texture_id;
    }

    unsigned int SceneToImgui::IDPick(SCENE_IMGUI scene, const int& mouseX, const int& mouseY)
    {
        if (scene == NO_SCENE)
            return std::numeric_limits<unsigned int>().max();

        GLint prevBinding{};
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevBinding);

        glBindFramebuffer(GL_FRAMEBUFFER, mScenes[scene].FBO);

        unsigned int goID{};
        glNamedFramebufferReadBuffer(mScenes[scene].FBO, GL_COLOR_ATTACHMENT1);
        glReadPixels(mouseX, mouseY, 1, 1,
            GL_RED_INTEGER, GL_UNSIGNED_INT, &goID);

        //std::stringstream ss;
        //ss << "ID: " << goID << "[" << mouseX << ',' << mouseY << "]";
        //CM_CORE_INFO(ss.str());

        if (RenderHelper::GetInstance()->mEditorWindowActive)
            RenderHelper::GetInstance()->mSelectedID = goID;
        if (goID >= MAX_ENTITIES)
            goID = 0;
            

        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, prevBinding);
        return goID;
    }

    void SceneToImgui::SetHovering(SCENE_IMGUI scene, bool hoverState)
    {
        mScenes[scene].isHovering = hoverState;
    }

    SceneToImgui::SCENE_IMGUI SceneToImgui::GetHovering()
    {
        for (int i{}; i < MAX_SCENES; ++i)
        {
            if (mScenes[i].isHovering)
                return static_cast<SCENE_IMGUI>(i);
        }
        return NO_SCENE;
    }



}


//// Rescales the Texture & RenderBuffers(how much pixels to draw) during runtime (when size chanages)
//void SceneToImgui::RescaleFramebuffer(float width, float height)
//{
//    glBindTexture(GL_TEXTURE_2D, texture_id);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
//
//    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//}