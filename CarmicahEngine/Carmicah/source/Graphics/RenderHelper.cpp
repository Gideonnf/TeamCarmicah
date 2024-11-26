#include "pch.h"
#include <glad/glad.h>
#include "RenderHelper.h"
#include "Components/Transform.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"
#include "Systems/AssetManager.h"
#include "Input/InputSystem.h"

namespace Carmicah
{
	unsigned int RenderHelper::mCapFontID{};
	std::queue<unsigned int> RenderHelper::mUnusedFontID{};


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
	screenMtx.translateThis(-1.f, -1.f).scaleThis(2 / screenWidth, 2 / screenHeight);

	mEditorCam.Pos(0,0);
	mEditorCam.Scale(100.f / screenWidth, 100.f / screenHeight);
}

void RenderHelper::UpdateEditorCam()
{
	static bool firstPressed = false;
	if (Input.IsMousePressed(MOUSE_BUTTON_RIGHT))
	{
		if (!firstPressed)
		{
			mOldMousePos = Input.GetMousePosition();
			firstPressed = true;
		}

		Vec2d mouseDiff = Input.GetMousePosition() - mOldMousePos;
		mEditorCam.PosXAdd(-static_cast<float>(mouseDiff.x) * mEditorCam.Scale().x * 0.5f);
		mEditorCam.PosYAdd(static_cast<float>(mouseDiff.y) * mEditorCam.Scale().y * 0.5f);
		mOldMousePos = Input.GetMousePosition();
	}
	else if (Input.IsMouseReleased(MOUSE_BUTTON_RIGHT))
	{
		firstPressed = false;
	}

	if (Input.IsKeyHold(KEY_EQUAL))
	{
		Vec2f& s = mEditorCam.GetScale();
		s.x += s.x * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
		s.y += s.x * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime());
	}
	else if (Input.IsKeyHold(KEY_MINUS))
	{
		Vec2f& s = mEditorCam.GetScale();
		s.x = std::fmaxf(s.x - s.x * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime()), 0.0001f);
		s.y = std::fmaxf(s.y - s.y * static_cast<float>(CarmicahTime::GetInstance()->GetDeltaTime()), 0.0001f);
	}
}

void RenderHelper::Render(std::optional<Transform*> cam, bool isEditor)
{
	glClearColor(0.75294f, 1.f, 0.93333f, 1.f); // Gideon's favourite
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Just did discard instead, cuz this stopped working
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Needs RBO to depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint mCurrShader{};


	const int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;

	BufferID currID(0, 0, std::numeric_limits<unsigned int>::max());
	// Loops through all batch Buffers
	for (const auto& it : mBufferMap)
	{
		const auto& batchBuffer = it.second;
		if (!(currID == it.first))
		{
			if (!isEditor && (it.first.dat[BUFFER_SHADER] == AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.debugShader).s))
				continue;

			GLint uniformLoc{};
			// If shader changed or camera changed
			if (currID.dat[BUFFER_SHADER] != it.first.dat[BUFFER_SHADER] || currID.dat[BUFFER_GAME_BASED] != it.first.dat[BUFFER_GAME_BASED])
			{
				mCurrShader = it.first.dat[BUFFER_SHADER];
				glUseProgram(mCurrShader);
				// Binds the entire 32 texture array
				glBindTexture(GL_TEXTURE_2D_ARRAY, AssetManager::GetInstance()->mArrayTex);

				// All Shaders have uNDC_to_Cam
				if (it.first.dat[BUFFER_GAME_BASED])
				{
					if (!cam.has_value())
						continue;
					// Handle Camera Transform[
					Mtx3x3f camSpace{};
					camSpace.scaleThis(cam.value()->Scale()).rotDegThis(cam.value()->Rot()).translateThis(-cam.value()->Pos());
					if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
						glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, camSpace.m);
				}
				else
				{
					if (UniformExists(mCurrShader, "uNDC_to_Cam", uniformLoc))
						glUniformMatrix3fv(uniformLoc, 1, GL_FALSE, screenMtx.m);
				}

			}
			// If font shader
			if (it.first.dat[BUFFER_SHADER] == AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.fontShader).s)
			{
				FontUniform* ft = GetFontUniforms(it.first.dat[BUFFER_ID]);
				if (ft != nullptr)
				{
					if (UniformExists(mCurrShader, "uTextColor", uniformLoc))
						glUniform3f(uniformLoc, ft->col[0], ft->col[1], ft->col[2]);
					if (UniformExists(mCurrShader, "uFontDisplace", uniformLoc))
						glUniform2f(uniformLoc, ft->offset.x, ft->offset.y);
					if (UniformExists(mCurrShader, "uFontScale", uniformLoc))
						glUniform2f(uniformLoc, ft->scale.x, ft->scale.y);

				}
			}

			currID = it.first;
		}

		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, batchBuffer.ibo);
		if (!batchBuffer.isDebug)
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
			for (auto& i : batchBuffer.buffer)
			{
				glBindVertexArray(i.vao);

				glMultiDrawElementsIndirect(GL_LINE_LOOP,
					GL_UNSIGNED_SHORT,// Indices
					(GLvoid*)0,
					mBatchSize,
					0);
			}
		}
	}

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

RenderHelper::FontUniform* RenderHelper::GetFontUniforms(const unsigned int& bufferID)
{
	auto fbtE = mFontBufferToEntity.find(bufferID);
	if (fbtE != mFontBufferToEntity.end())
	{
		auto ftU = mFontUniforms.find(fbtE->second);
		if (ftU != mFontUniforms.end())
		{
			return &ftU->second;
		}
	}
	return nullptr;
}

unsigned int RenderHelper::AssignFont(const unsigned int& e)
{
	FontUniform ftU{};
	if (mUnusedFontID.size() != 0)
	{
		ftU.bufferID = mUnusedFontID.front();
		mUnusedFontID.pop();
	}
	else
		ftU.bufferID = mCapFontID++;
	mFontBufferToEntity.emplace(ftU.bufferID, e);
	mFontUniforms.emplace(e, ftU);
	
	return ftU.bufferID;
}

void RenderHelper::UnassignFont(const unsigned int& e)
{
	auto ftU = mFontUniforms.find(e);
	if (ftU != mFontUniforms.end())
	{
		auto fbtE = mFontBufferToEntity.find(ftU->second.bufferID);
		if (fbtE != mFontBufferToEntity.end())
			mFontBufferToEntity.erase(fbtE->first);

		mUnusedFontID.push(ftU->first);
		mFontUniforms.erase(e);
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