/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			BaseGraphicsSystem.cpp

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Similar function used across all rendering systems to check if uniform exists in shader

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#include "pch.h"
#include <glad/glad.h>
#include "Graphics/BaseGraphicsSystem.h"
#include "log.h"

namespace Carmicah
{
	bool BaseGraphicsSystem::UniformExists(const GLuint& shdr, const char* str, GLint& ref)
	{
		ref = glGetUniformLocation(shdr, str);
		if (ref >= 0)
			return true;

		std::stringstream ss;
		ss << "Uniform variable: " << str << " dosen't exist!!!\n";
		CM_CORE_ERROR(ss.str());

		return false;
	}

	float BaseGraphicsSystem::CalcDepth(const float& depth)
	{
		const float furtherstDepth{ -100.f }, nearestDepth{ 100.f };
		return -(depth + nearestDepth) / (nearestDepth - furtherstDepth);
	}

	void BaseGraphicsSystem::RenderPrimitive(const Primitive& p)
	{
		glBindVertexArray(p.vaoid);

		switch (p.drawMode)
		{
		case GL_LINE_LOOP:
			glLineWidth(2.f);
			glDrawArrays(GL_LINE_LOOP, 0, p.drawCnt);
			break;
		case GL_TRIANGLES:
			glDrawElements(GL_TRIANGLES, p.drawCnt, GL_UNSIGNED_SHORT, NULL);
			break;
		case GL_TRIANGLE_FAN:
			glDrawArrays(GL_TRIANGLE_FAN, 0, p.drawCnt);
			break;
		}
	}
}