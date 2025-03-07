/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RenderHelper.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Handles the actual rendering of all entities, and also the batching and instancing of different entities
				Also handles the Rendering of Gizmos when in editor mode

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "RenderHelper.h"
#include "Components/Transform.h"
#include "Components/UITransform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "Systems/AssetManager.h"
#include "Input/InputSystem.h"
#include "Editor/HierarchyWindow.h"
#include "Editor/SceneToImgui.h"

namespace Carmicah
{
	float RenderHelper::zeroFiller[4]{ 0.f,0.f,0.f,0.f };
	float RenderHelper::oneFiller[4]{ 1.f,1.f,1.f,1.f };

bool RenderHelper::UniformExists(const GLuint& shdr, const char* str, GLint& ref)
{
	ref = glGetUniformLocation(shdr, str);
	if (ref >= 0)
		return true;

	std::stringstream ss;
	ss << "Uniform variable: " << str << " dosen't exist!!!\n";
	CM_CORE_ERROR(ss.str());

	return false;
}

void RenderHelper::InitScreenDimension(const float& screenWidth, const float& screenHeight)
{
	Mtx33Identity(screenMtx);
	screenMtx.translateThis(-1.f, -1.f).scaleThis(2.f / screenWidth, 2.f / screenHeight);

	mEditorCam.Pos(0,0);
	mEditorCam.Scale(100.f / screenWidth, 100.f / screenHeight);
}

void RenderHelper::UpdateEditorCam()
{
	if (Input.IsMousePressed(MOUSE_BUTTON_RIGHT))
		mOldMousePos = Input.GetMousePosition();
	else if (Input.IsMouseHold(MOUSE_BUTTON_RIGHT))
	{
		Vec2d mouseDiff = Input.GetMousePosition() - mOldMousePos;

		mEditorCam.PosXAdd(-static_cast<float>(mouseDiff.x) / AssetManager::GetInstance()->enConfig.Width / mEditorCam.Scale().x * 2.f);
		mEditorCam.PosYAdd(static_cast<float>(mouseDiff.y) / AssetManager::GetInstance()->enConfig.Height / mEditorCam.Scale().y * 2.f);
		mOldMousePos = Input.GetMousePosition();
	}

	if (SceneToImgui::GetInstance()->GetHovering() == SceneToImgui::EDITOR_SCENE && abs(Input.GetScrollOffset()) > DBL_EPSILON)
	{
		Vec2f& s = mEditorCam.GetScale();
		float ratio = s.x / s.y;
		if (Input.GetScrollOffset() > 0.f)
		{
			s.x += EDITOR_ZOOM_SPEED * Input.GetScrollOffset() * ratio * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
			s.y += EDITOR_ZOOM_SPEED * Input.GetScrollOffset() * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
		}
		else
		{
			s.x = std::fmaxf(s.x + EDITOR_ZOOM_SPEED * Input.GetScrollOffset() * ratio * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime()), MIN_HEIGHT_SCALE * ratio);
			s.y = std::fmaxf(s.y + EDITOR_ZOOM_SPEED * Input.GetScrollOffset() * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime()), MIN_HEIGHT_SCALE);
		}
	}
	else if (Input.IsKeyHold(KEY_EQUAL))
	{
		Vec2f& s = mEditorCam.GetScale();
		float ratio = s.x / s.y;

		s.x += EDITOR_ZOOM_SPEED * ratio * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
		s.y += EDITOR_ZOOM_SPEED * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
	}
	else if (Input.IsKeyHold(KEY_MINUS))
	{
		Vec2f& s = mEditorCam.GetScale();
		float ratio = s.x / s.y;

		s.x = std::fmaxf(s.x - EDITOR_ZOOM_SPEED * ratio * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime()), MIN_HEIGHT_SCALE * ratio);
		s.y = std::fmaxf(s.y - EDITOR_ZOOM_SPEED * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime()), MIN_HEIGHT_SCALE);
	}

}

void RenderHelper::Render(std::optional<Transform*> cam, bool isEditor)
{
	glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite

	static unsigned int debugShaderID = AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.debugShader).s;
	static unsigned int defaultShaderID = AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.defaultShader).s;

	GLuint mCurrShader{};
	const int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;
	SceneToImgui::FBOScene FBOScene = SceneToImgui::GetInstance()->GetCurrentFramebuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, FBOScene.FBO);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);	// Write to depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, 1, zeroFiller);

	for (int renderPass = 0; renderPass < 2; ++renderPass)
	{
		BufferID currID(0, std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned int>::max());
		if (renderPass == 1)
		{
			glClearBufferfv(GL_COLOR, 2, zeroFiller);
			glClearBufferfv(GL_COLOR, 3, oneFiller);
		}
		// Loops through all batch Buffers
		for (const auto& it : mBufferMap)
		{
			const auto& batchBuffer = it.second;
			if (!(currID == it.first))
			{
				if (!isEditor && (it.first.dat[BUFFER_SHADER] == debugShaderID))
					continue;
				if (renderPass != 0 && it.first.dat[BUFFER_SHADER] != defaultShaderID)
					continue;

				GLint uniformLoc{};
				// If shader changed or camera changed
				if (currID.dat[BUFFER_SHADER] != it.first.dat[BUFFER_SHADER] || currID.dat[BUFFER_GAME_BASED] != it.first.dat[BUFFER_GAME_BASED])
				{
					mCurrShader = it.first.dat[BUFFER_SHADER];
					glUseProgram(mCurrShader);

					// if Basic Shader, ie. the only shader using OIT (Order Independent Transparancy[Weighted])
					if (it.first.dat[BUFFER_SHADER] == defaultShaderID)
					{
						if(UniformExists(mCurrShader, "uPassNum", uniformLoc))
							glUniform1i(uniformLoc, renderPass);
						if (renderPass == 0)
						{
							glDisable(GL_BLEND);

							glEnable(GL_DEPTH_TEST);

							// Specifically disable writting to ID 2 since id 1 is particles, yes, ik, it's dumb
							if(it.first.dat[BUFFER_ID] == 0)
								glColorMaski(1, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
							else if(it.first.dat[BUFFER_ID] == 2)
								glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
							glDepthMask(GL_TRUE);	// Enable writing to depth buffer
							glDepthFunc(GL_LESS);	// When incoming depth is smaller, pass the test

						}
						else
						{
							glEnable(GL_BLEND);
							glBlendFunci(0, GL_ZERO, GL_ONE);
							glBlendFunci(1, GL_ZERO, GL_ONE);
							glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
							glBlendFunci(2, GL_ONE, GL_ONE);	// left - the amount of alpha of the newly rendered thing, right - the original thing at the back
							glBlendFunci(3, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
							glBlendEquation(GL_FUNC_ADD);

							glDepthMask(GL_FALSE);
							glDepthFunc(GL_LEQUAL);
						}
					}
					else
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glColorMaski(1, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
						glEnable(GL_DEPTH_TEST);
						glDepthMask(GL_TRUE);
						glDepthFunc(GL_LESS);
					}
					// Binds the entire 32 texture array
					glBindTexture(GL_TEXTURE_2D_ARRAY, AssetManager::GetInstance()->mArrayTex);

					// All Shaders have uNDC_to_Cam
					if (it.first.dat[BUFFER_GAME_BASED])
					{
						if (!cam.has_value())
							continue;
						// Handle Camera Transform[
						Mtx3x3f camSpace{};
						camSpace.lookAtDeg(cam.value()->Pos(), cam.value()->Rot(), cam.value()->Scale());
						if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
							glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, camSpace.m);
					}
					else
					{
						if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
							glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, screenMtx.m);
					}
					// Reset for this mode
					if (it.first.dat[BUFFER_SHADER] == defaultShaderID &&
						it.first.dat[BUFFER_ID] == 0)
					{
						if (UniformExists(mCurrShader, "uIsText", uniformLoc))
							glUniform1i(uniformLoc, 0);
					}
				}

				currID = it.first;
			}

			// Rendering part
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, batchBuffer.ibo);
			if (it.first.dat[BUFFER_ID] == 1)
			{
				// Specifically for rendering fonts / particles
				if (it.first.dat[BUFFER_SHADER] == defaultShaderID)
				{
					if (mFontData.empty() || mFontData.begin()->second.vtxSize == 0)
						continue;
					GLint uniformLoc{};
					if (UniformExists(mCurrShader, "uIsText", uniformLoc))
						glUniform1i(uniformLoc, 1);


					auto& p{ AssetManager::GetInstance()->GetAsset<Primitive>("Square") };

					for (auto& font : mFontData)
					{
						if (UniformExists(mCurrShader, "uTextColor", uniformLoc))
							glUniform3f(uniformLoc, font.second.col[0], font.second.col[1], font.second.col[2]);
						if (UniformExists(mCurrShader, "uTextOffset", uniformLoc))
							glUniform2f(uniformLoc, font.second.offset.x, font.second.offset.y);
						if (UniformExists(mCurrShader, "uTextScale", uniformLoc))
							glUniform2f(uniformLoc, font.second.scale.x, font.second.scale.y);

						auto& buff = batchBuffer.buffer[0];
						int bSize = static_cast<int>(p.vtx.size() * mBatchSize);
						glBindVertexArray(buff.vao);

						for(int off = 0; off < static_cast<int>(font.second.vtx.size()); off += bSize)
						{
							int drawCnt = std::min(font.second.vtxSize, mBatchSize);
							if (drawCnt == 0)
								break;
							int vtxCnt = std::min(drawCnt * static_cast<int>(p.vtx.size()), bSize);

							glNamedBufferSubData(buff.vbo, sizeof(vtxTexd2D) * off, sizeof(vtxTexd2D) * vtxCnt, font.second.vtx.data() + off);

							glMultiDrawElementsIndirect(GL_TRIANGLES,
								GL_UNSIGNED_SHORT,// Indices
								(GLvoid*)0,
								drawCnt,
								0);
						}
					}
				}
			}
			else if (!batchBuffer.isDebug)
			{
				for (auto& i : batchBuffer.buffer)
				{
					glBindVertexArray(i.vao);

					glMultiDrawElementsIndirect(GL_TRIANGLES,
						GL_UNSIGNED_SHORT,// Indices
						(GLvoid*)0,
						mBatchSize,
						0);
				}
			}
			else
			{
				glLineWidth(4.f);

				for (auto& i : batchBuffer.buffer)
				{
					glBindVertexArray(i.vao);

					glMultiDrawElementsIndirect(batchBuffer.pRef->drawMode,
						GL_UNSIGNED_SHORT,// Indices
						(GLvoid*)0,
						mBatchSize,
						0);
				}
			}
		}
	}

	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunci(1, GL_ZERO, GL_ONE);

	mCurrShader = AssetManager::GetInstance()->GetAsset<Shader>("combi").s;
	glUseProgram(mCurrShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOScene.accumTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, FBOScene.revealTex);

	GLModel& ScrnQuad = AssetManager::GetInstance()->GetAsset<GLModel>("Quad");
	glBindVertexArray(ScrnQuad.vao);
	glDrawArrays(ScrnQuad.primitive, 0, ScrnQuad.drawCnt);


	// Gizmos
	if (isEditor)
		RenderGizmos();

	glBindVertexArray(0);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

void RenderHelper::Render(const unsigned int& cam, bool isEditor)
{
	if (ComponentManager::GetInstance()->HasComponent<Transform>(cam))
	{
		// Handle Camera Transform
		Transform& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
		Render(std::optional<Transform*>{&camTrans}, isEditor);
	}
	else
		Render(std::nullopt, isEditor);
}

void RenderHelper::FinalRender()
{
	GLuint mCurrShader{ AssetManager::GetInstance()->GetAsset<Shader>("installer").s };
	GLint uniformLoc{};
	UNUSED(uniformLoc);
	SceneToImgui::FBOScene FBOScene = SceneToImgui::GetInstance()->GetCurrentFramebuffer();


	glUseProgram(mCurrShader);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//if (UniformExists(mCurrShader, "screen", uniformLoc))
	//	glUniform1i(uniformLoc, FBOScene.FBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBOScene.texture_id);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLModel& ScrnQuad = AssetManager::GetInstance()->GetAsset<GLModel>("QuadTex");
	glBindVertexArray(ScrnQuad.vao);
	glDrawArrays(ScrnQuad.primitive, 0, ScrnQuad.drawCnt);

}

void RenderHelper::AssignFont(const unsigned int& e)
{
	BufferCPUSide buff{};
	mFontData.emplace(e, buff);
	buff.vtx.resize(400);
}

void RenderHelper::ReserveFontBuffer(BufferCPUSide& buff, const size_t& sz)
{
	if (buff.vtx.size() < sz * 4)
	{
		size_t numBatches = sz / 100 + (sz % 100 > 0);

		buff.vtx.resize(numBatches * 400);
	}
}

void RenderHelper::UnassignFont(const unsigned int& e)
{
	auto buff = mFontData.find(e);
	if (buff != mFontData.end())
		mFontData.erase(buff);
}

void RenderHelper::LoadGizmos()
{
	if(AssetManager::GetInstance()->AssetExist<Primitive>("Circle"))
	{
		Primitive p = AssetManager::GetInstance()->GetAsset<Primitive>("Circle");

		GLModel circle;
		glCreateBuffers(1, &circle.vbo);
		glNamedBufferStorage(circle.vbo, p.vtx.size() * sizeof(Vec2f), p.vtx.data(), GL_MAP_WRITE_BIT);
		glCreateVertexArrays(1, &circle.vao);
		glEnableVertexArrayAttrib(circle.vao, 0);
		glVertexArrayVertexBuffer(circle.vao, 0, circle.vbo, 0, sizeof(Vec2f));
		glVertexArrayAttribFormat(circle.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(circle.vao, 0, 0);
		circle.primitive = GL_TRIANGLE_FAN;
		circle.drawCnt = p.drawCnt;
		AssetManager::GetInstance()->AddAsset<GLModel>("Circle", circle);
	}
	if (AssetManager::GetInstance()->AssetExist<BasePrimitive>("DebugLine"))
	{
		BasePrimitive p = AssetManager::GetInstance()->GetAsset<BasePrimitive>("DebugLine");
		GLModel line;
		glCreateBuffers(1, &line.vbo);
		glNamedBufferStorage(line.vbo, p.vtx.size() * sizeof(Vec2f), p.vtx.data(), GL_MAP_WRITE_BIT);
		glCreateVertexArrays(1, &line.vao);
		glEnableVertexArrayAttrib(line.vao, 0);
		glVertexArrayVertexBuffer(line.vao, 0, line.vbo, 0, sizeof(Vec2f));
		glVertexArrayAttribFormat(line.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(line.vao, 0, 0);
		line.primitive = GL_LINES;
		line.drawCnt = p.drawCnt;
		AssetManager::GetInstance()->AddAsset<GLModel>("Line", line);
	}
	if (AssetManager::GetInstance()->AssetExist<BasePrimitive>("DebugDot"))
	{
		BasePrimitive p = AssetManager::GetInstance()->GetAsset<BasePrimitive>("DebugDot");
		GLModel dot;
		glCreateBuffers(1, &dot.vbo);
		glNamedBufferStorage(dot.vbo, p.vtx.size() * sizeof(Vec2f), p.vtx.data(), GL_MAP_WRITE_BIT);
		glCreateVertexArrays(1, &dot.vao);
		glEnableVertexArrayAttrib(dot.vao, 0);
		glVertexArrayVertexBuffer(dot.vao, 0, dot.vbo, 0, sizeof(Vec2f));
		glVertexArrayAttribFormat(dot.vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(dot.vao, 0, 0);
		dot.primitive = GL_POINTS;
		dot.drawCnt = p.drawCnt;
		AssetManager::GetInstance()->AddAsset<GLModel>("Dot", dot);
	}
}

void RenderHelper::RenderGizmos()
{
	if (!mEditorWindowActive || HierarchyWindow::selectedGO == nullptr || mEditorMode == GIZMOS_MODE::GIZMOS_NONE)
		return;

	GLuint mCurrShader{ AssetManager::GetInstance()->GetAsset<Shader>("gizmo").s };
	GLint uniformLoc{};
	glUseProgram(mCurrShader);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaski(1, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);


	Mtx3x3f mtx{};
	Vec2f translation{};
	Vec2f gizmoScale{ GIZMO_SCALE / mEditorWindomDim.x, GIZMO_SCALE / mEditorWindomDim.y };
	float lineWitdh = 10.f;

	if (HierarchyWindow::selectedGO->HasComponent<Transform>())
	{
		Transform& t = HierarchyWindow::selectedGO->GetComponent<Transform>();
		Vec2f selectedTrans{t.worldSpace.m20, t.worldSpace.m21};
		translation = selectedTrans - mEditorCam.Pos();
		Mtx3x3f temp{};
		temp.scaleThis(1.f / gizmoScale.x, 1.f / gizmoScale.y).scaleThis(mEditorCam.GetScale());
		translation = temp * translation;
	}
	else if (HierarchyWindow::selectedGO->HasComponent<UITransform>())
	{
		translation = HierarchyWindow::selectedGO->GetComponent<UITransform>().ExtractWorldPos();
		Mtx3x3f temp{};
		temp.scaleThis(1.f / gizmoScale.x, 1.f / gizmoScale.y);
		translation = temp * screenMtx * translation;
	}


	switch (mEditorMode)
	{
	case GIZMOS_MODE::GIZMOS_ROTATE:
	{
		// Rotation
		mtx.scaleThis(gizmoScale).translateThis(translation);
		if (UniformExists(mCurrShader, "uMtx", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, mtx.m);

		if (UniformExists(mCurrShader, "uColor", uniformLoc))
			glUniform4f(uniformLoc, 1.0f, 0.0f, 0.0f, 0.25);
		if (UniformExists(mCurrShader, "uID", uniformLoc))
			glUniform1ui(uniformLoc, std::numeric_limits<unsigned int>().max());

		GLModel& circle = AssetManager::GetInstance()->GetAsset<GLModel>("Circle");
		glBindVertexArray(circle.vao);
		glDrawArrays(circle.primitive, 0, circle.drawCnt);

		break;
	}
	case GIZMOS_MODE::GIZMOS_SCALE:
	{
		glPointSize(lineWitdh * 2.f);
		GLModel& dot = AssetManager::GetInstance()->GetAsset<GLModel>("Dot");
		glBindVertexArray(dot.vao);
		// x_Dot (Red)
		Mtx33Identity(mtx);
		mtx.scaleThis(gizmoScale).translateThis(0.5f, 0.0f).translateThis(translation);
		if (UniformExists(mCurrShader, "uMtx", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, mtx.m);
		if (UniformExists(mCurrShader, "uColor", uniformLoc))
			glUniform4f(uniformLoc, 1.0f, 0.0f, 0.0f, 1.0);
		if (UniformExists(mCurrShader, "uID", uniformLoc))
			glUniform1ui(uniformLoc, std::numeric_limits<unsigned int>().max() - 1);
		glDrawArrays(dot.primitive, 0, dot.drawCnt);
		// y_Dot (Green)
		Mtx33Identity(mtx);
		mtx.scaleThis(gizmoScale).translateThis(0.0f, 0.5f).translateThis(translation);
		if (UniformExists(mCurrShader, "uMtx", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, mtx.m);
		if (UniformExists(mCurrShader, "uColor", uniformLoc))
			glUniform4f(uniformLoc, 0.0f, 1.0f, 0.0f, 1.0);
		if (UniformExists(mCurrShader, "uID", uniformLoc))
			glUniform1ui(uniformLoc, std::numeric_limits<unsigned int>().max());
		glDrawArrays(dot.primitive, 0, dot.drawCnt);
		[[fallthrough]];
	}
	case GIZMOS_MODE::GIZMOS_TRANSLATE:
	{
		glLineWidth(lineWitdh);
		GLModel& line = AssetManager::GetInstance()->GetAsset<GLModel>("Line");
		glBindVertexArray(line.vao);

		// y_line (Green)
		Mtx33Identity(mtx);
		mtx.scaleThis(gizmoScale).translateThis(translation).rotDegThis(90.f);
		if (UniformExists(mCurrShader, "uMtx", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, mtx.m);
		if (UniformExists(mCurrShader, "uColor", uniformLoc))
			glUniform4f(uniformLoc, 0.0f, 1.0f, 0.0f, 1.0);
		if (UniformExists(mCurrShader, "uID", uniformLoc))
			glUniform1ui(uniformLoc, std::numeric_limits<unsigned int>().max());
		glDrawArrays(line.primitive, 0, line.drawCnt);
		// x_line (Red)
		Mtx33Identity(mtx);
		mtx.scaleThis(gizmoScale).translateThis(translation);
		if (UniformExists(mCurrShader, "uMtx", uniformLoc))
			glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, mtx.m);
		if (UniformExists(mCurrShader, "uColor", uniformLoc))
			glUniform4f(uniformLoc, 1.0f, 0.0f, 0.0f, 1.0);
		if (UniformExists(mCurrShader, "uID", uniformLoc))
			glUniform1ui(uniformLoc, std::numeric_limits<unsigned int>().max() - 1);
		glDrawArrays(line.primitive, 0, line.drawCnt);
		break;
	}
	}
}

//void BaseGraphicsSystem::RenderPrimitive(const Primitive& p)
//{
//	glBindVertexArray(p.vaoid);
//
//	switch (p.drawMode)
//	{
//	case GL_LINE_LOOP:
//		glLineWidth(2.f);
//		glDrawArrays(GL_LINE_LOOP, 0, p.drawCnt);
//		break;
//	case GL_TRIANGLES:
//		glDrawElements(GL_TRIANGLES, p.drawCnt, GL_UNSIGNED_SHORT, NULL);
//		break;
//	case GL_TRIANGLE_FAN:
//		glDrawArrays(GL_TRIANGLE_FAN, 0, p.drawCnt);
//		break;
//	}
//}

}