/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			BaseGraphicsSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Similar function used across all rendering systems to check if uniform exists in shader

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef BASE_GRAPHICS_SYSTEM
#define BASE_GRAPHICS_SYSTEM
class BaseGraphicsSystem
{
protected:
	/*
	brief
		Checks if uniform exists in the shader
	param[shdr]
		shader reference
	param[str]
		name to check
	param[ref]
		the uniform reference to return
	return
		if the check succeeded
	*/
	bool UniformExists(const GLuint& shdr, const char* str, GLint& ref);

	float CalcDepth(const float& depth);
};
#endif