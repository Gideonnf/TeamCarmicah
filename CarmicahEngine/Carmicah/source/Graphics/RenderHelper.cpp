#include "pch.h"
#include <glad/glad.h>
#include "RenderHelper.h"
#include "ECS/ComponentManager.h"
#include "Systems/AssetManager.h"
#include "Components/Transform.h"
#include "Systems/AssetManager.h"

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
}

void RenderHelper::Render(const unsigned int& cam)
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
		if(!(currID == it.first))
		{
			GLint uniformLoc{};
			// If shader changed or camera changed
			if (currID.dat[SHADER] != it.first.dat[SHADER] || currID.dat[GAME_BASED] != it.first.dat[GAME_BASED])
			{
				mCurrShader = it.first.dat[SHADER];
				glUseProgram(mCurrShader);
				// Binds the entire 32 texture array
				glBindTexture(GL_TEXTURE_2D_ARRAY, AssetManager::GetInstance()->mArrayTex);

				// All Shaders have uNDC_to_Cam
				if (it.first.dat[GAME_BASED] && ComponentManager::GetInstance()->HasComponent<Transform>(cam))
				{
					// Handle Camera Transform
					auto& camTrans = ComponentManager::GetInstance()->GetComponent<Transform>(cam);
					Mtx3x3f camSpace{};
					camSpace.scaleThis(camTrans.Scale()).rotDegThis(-camTrans.Rot()).translateThis(-camTrans.Pos());
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
			if (it.first.dat[SHADER] == AssetManager::GetInstance()->GetAsset<Shader>(AssetManager::GetInstance()->enConfig.fontShader).s)
			{
				if (mFontUniforms.find(it.first.dat[ID]) != mFontUniforms.end())
				{
					FontUniform& ft = mFontUniforms.find(it.first.dat[ID])->second;

					if (UniformExists(mCurrShader, "uTextColor", uniformLoc))
						glUniform3f(uniformLoc, ft.col[0], ft.col[1], ft.col[2]);
					if (UniformExists(mCurrShader, "uFontDisplace", uniformLoc))
						glUniform2f(uniformLoc, ft.offset.x, ft.offset.y);

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

void RenderHelper::UnassignFont(const unsigned int& e)
{
	/*
	auto& ftU = mFontUniforms.find(e);
	if (ftU != mFontUniforms.end())
	{
		//mBufferMap.find(ftU->second.BufferID);
	}
	*/
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