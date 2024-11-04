/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        SceneToImgui.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       A bridge to map the graphics rendering to ImGui. Binds rendering to a texture that can be mapped to an image in ImGui

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#include "pch.h"
#include <glad/glad.h>
#include "SceneToImgui.h"
#include "log.h"
#include "Input/InputSystem.h"
#include <sstream>

namespace Carmicah
{
    void SceneToImgui::CreateFramebuffer(int width, int height)
    {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);
        glTextureStorage2D(texture_id, 1, GL_RGBA8, width, height);
        glTextureParameterf(texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameterf(texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture_id, 0);

        glGenTextures(1, &picker_id);
        glBindTexture(GL_TEXTURE_2D, picker_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
        glTextureParameterf(picker_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameterf(picker_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, picker_id, 0);

        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void SceneToImgui::UnloadFramebuffer()
    {
        glDeleteRenderbuffers(1, &RBO);
        glDeleteTextures(1, &picker_id);
        glDeleteTextures(1, &texture_id);
        glDeleteFramebuffers(1, &FBO);
    }

    void SceneToImgui::BindFramebuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
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

    unsigned int SceneToImgui::IDPick(const int& mouseX, const int& mouseY)
    {
        GLint prevBinding{};
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevBinding);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        unsigned int goID{};
        glNamedFramebufferReadBuffer(FBO, GL_COLOR_ATTACHMENT1);
        glReadPixels(mouseX, mouseY, 1, 1,
            GL_RED_INTEGER, GL_UNSIGNED_INT, &goID);

        // basically if ID Dun exists tq
        if (goID != 0 && goID != 1061208237)
        {
            std::stringstream ss;
            ss << "ID: " << goID << "[" << mouseX << ',' << mouseY << "]";
            CM_CORE_INFO(ss.str());
        }
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, prevBinding);
        return goID;
    }


    // Rescales the Texture & RenderBuffers(how much pixels to draw) during runtime (when size chanages)
    void SceneToImgui::RescaleFramebuffer(float width, float height)
    {
        width = height;
        //glBindTexture(GL_TEXTURE_2D, texture_id);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);

        //glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height);
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    }
}