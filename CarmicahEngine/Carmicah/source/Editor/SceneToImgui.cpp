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
#include "Systems/AssetManager.h"

namespace Carmicah
{
    void SceneToImgui::CreateScreenQuad()
    {
        if (!AssetManager::GetInstance()->AssetExist<GLModel>("Quad"))
        {
            GLModel quad;
            //GLenum primitive{};
            //GLuint drawCnt{};
            //GLuint vao{};
            //GLuint vbo{};
            std::vector<Vec2f> p
            {
                Vec2f(1.f, -1.0f), Vec2f(1.0f, 1.0f),
                Vec2f(-1.0f, -1.0f), Vec2f(-1.0f, 1.0f)
            };

            glCreateBuffers(1, &quad.vbo);
            glNamedBufferStorage(quad.vbo, p.size() * sizeof(Vec2f), p.data(), GL_MAP_WRITE_BIT);
            glCreateVertexArrays(1, &quad.vao);
            // layout=0
            glEnableVertexArrayAttrib(quad.vao, 0);
            glVertexArrayVertexBuffer(quad.vao, 0, quad.vbo, 0, sizeof(Vec2f));
            glVertexArrayAttribFormat(quad.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(quad.vao, 0, 0);

            GLuint ebo;
            std::vector<GLushort> idx_vtx{ 0,1,2,3 };
            glCreateBuffers(1, &ebo);
            glNamedBufferStorage(ebo, sizeof(GLushort) * 4, reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_MAP_WRITE_BIT);
            glVertexArrayElementBuffer(quad.vao, ebo);

            quad.primitive = GL_TRIANGLE_STRIP;
            quad.drawCnt = 4;
            AssetManager::GetInstance()->AddAsset<GLModel>("Quad", quad);
        }
        if (!AssetManager::GetInstance()->AssetExist<GLModel>("QuadTex"))
        {
            GLModel quad;
            //GLenum primitive{};
            //GLuint drawCnt{};
            //GLuint vao{};
            //GLuint vbo{};
            std::vector<Vec2f> p
            {
                Vec2f(1.f, -1.0f), Vec2f(1.0f, 1.0f),
                Vec2f(-1.0f, -1.0f), Vec2f(-1.0f, 1.0f),
                Vec2f(1.f, 0.0f), Vec2f(1.0f, 1.0f),
                Vec2f(0.0f, 0.0f), Vec2f(0.0f, 1.0f)
            };

            glCreateBuffers(1, &quad.vbo);
            glNamedBufferStorage(quad.vbo, p.size() * sizeof(Vec2f), p.data(), GL_MAP_WRITE_BIT);
            glCreateVertexArrays(1, &quad.vao);
            // layout=0
            glEnableVertexArrayAttrib(quad.vao, 0);
            glVertexArrayVertexBuffer(quad.vao, 0, quad.vbo, 0, sizeof(Vec2f));
            glVertexArrayAttribFormat(quad.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(quad.vao, 0, 0);
            // layout=1
            glEnableVertexArrayAttrib(quad.vao, 1);
            glVertexArrayVertexBuffer(quad.vao, 1, quad.vbo, sizeof(Vec2f) * 4, sizeof(Vec2f));
            glVertexArrayAttribFormat(quad.vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(quad.vao, 1, 1);

            GLuint ebo;
            std::vector<GLushort> idx_vtx{ 0,1,2,3 };
            glCreateBuffers(1, &ebo);
            glNamedBufferStorage(ebo, sizeof(GLushort) * 4, reinterpret_cast<GLvoid*>(idx_vtx.data()), GL_MAP_WRITE_BIT);
            glVertexArrayElementBuffer(quad.vao, ebo);

            quad.primitive = GL_TRIANGLE_STRIP;
            quad.drawCnt = 4;
            AssetManager::GetInstance()->AddAsset<GLModel>("QuadTex", quad);
        }
    }

    void SceneToImgui::CreateFramebuffer(int width, int height)
    {
        CreateScreenQuad();

        for (int i{}; i < MAX_SCENES; ++i)
        {
            // Make the Base FBO that is being used
            glGenFramebuffers(1, &mScenes[i].FBO);
            glBindFramebuffer(GL_FRAMEBUFFER, mScenes[i].FBO);

            glCreateTextures(GL_TEXTURE_2D, 1, &mScenes[i].texture_id);
            glTextureStorage2D(mScenes[i].texture_id, 1, GL_RGBA16F, width, height);
            glTextureParameterf(mScenes[i].texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameterf(mScenes[i].texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mScenes[i].texture_id, 0);

            glCreateTextures(GL_TEXTURE_2D, 1, &mScenes[i].picker_id);
            glTextureStorage2D(mScenes[i].picker_id, 1, GL_R32UI, width, height);
            glTextureParameterf(mScenes[i].picker_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameterf(mScenes[i].picker_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, mScenes[i].picker_id, 0);

            glCreateTextures(GL_TEXTURE_2D, 1, &mScenes[i].depthTex);
            glTextureStorage2D(mScenes[i].depthTex, 1, GL_DEPTH_COMPONENT32F, width, height);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mScenes[i].depthTex, 0);

            //glGenRenderbuffers(1, &mScenes[i].RBO);
            //glBindRenderbuffer(GL_RENDERBUFFER, mScenes[i].RBO);
            //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mScenes[i].RBO);

            // Transparent Textures
            glCreateTextures(GL_TEXTURE_2D, 1, &mScenes[i].accumTex);
            glTextureStorage2D(mScenes[i].accumTex, 1, GL_RGBA16F, width, height);
            glTextureParameterf(mScenes[i].accumTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameterf(mScenes[i].accumTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, mScenes[i].accumTex, 0);

            glCreateTextures(GL_TEXTURE_2D, 1, &mScenes[i].revealTex);
            glTextureStorage2D(mScenes[i].revealTex, 1, GL_R8, width, height);
            glTextureParameterf(mScenes[i].revealTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameterf(mScenes[i].revealTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, mScenes[i].revealTex, 0);

            unsigned int drawBuffers[] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3
            };
            glDrawBuffers(sizeof(drawBuffers) / sizeof(unsigned int), drawBuffers);


#ifndef CM_RELEASE
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                CM_CORE_ERROR(std::string{ "ERROR::FRAMEBUFFER:: Framebuffer(" } + std::to_string(i) + ") is not complete!\n");
            }
#endif
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        //glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void SceneToImgui::UnloadFramebuffer()
    {
        for (int i{}; i < MAX_SCENES; ++i)
        {
            //glDeleteRenderbuffers(1, &mScenes[i].RBO);
            glDeleteTextures(1, &mScenes[i].picker_id);
            glDeleteTextures(1, &mScenes[i].texture_id);
            glDeleteTextures(1, &mScenes[i].depthTex);
            glDeleteTextures(1, &mScenes[i].accumTex);
            glDeleteTextures(1, &mScenes[i].revealTex);
            glDeleteFramebuffers(1, &mScenes[i].FBO);
        }
    }

    void SceneToImgui::SelectFrameBuffer(SCENE_IMGUI scene)
    {
        currDrawingScene = scene;
    }

    SceneToImgui::FBOScene SceneToImgui::GetCurrentFramebuffer()
    {
        if (currDrawingScene == NO_SCENE)
        {
            UnbindFramebuffer();
            return mScenes[0];
        }
        return mScenes[currDrawingScene];
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

        if(mouseX > 1920 || mouseX < 0 || mouseY > 1080 || mouseY < 0)
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

    void SceneToImgui::SelectMouseIDObjPick()
    {
        Vec2i mousePosI = { static_cast<int>(Input.GetMousePosition().x), 1080 - static_cast<int>(Input.GetMousePosition().y) };
#ifndef CM_INSTALLER
        SceneToImgui::SCENE_IMGUI currScene = SceneToImgui::GetInstance()->GetHovering();
        if (currScene == SceneToImgui::NO_SCENE)
        {
            return;
        }
        mIDHovered = IDPick(currScene, mousePosI.x, mousePosI.y);
#else
        mIDHovered = IDPick(GAME_SCENE, mousePosI.x, mousePosI.y);
#endif
    }

    unsigned int SceneToImgui::GetIDObjPick()
    {
        return mIDHovered;
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