/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RenderHelper.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Render Helper handles the rendering order of textured meshes onto the screen

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include <map>
#include "Singleton.h"
#include "Systems/AssetTypes.h"

namespace Carmicah
{
	class RenderHelper : public Singleton<RenderHelper>
	{
	private:
		/*!*************************************************************************
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
		***************************************************************************/
		bool UniformExists(const GLuint& shdr, const char* str, GLint& ref);

		Mtx3x3f screenMtx{};	// Calculated Screen Matrix multiplier to use for UI rendering
		
		static unsigned int mCapFontID;
		static std::queue<unsigned int> mUnusedFontID;
		std::map<unsigned int, unsigned int> mFontBufferToEntity;
	public:
		enum BUFFER_BITS : unsigned char
		{
			SHADER = 0,
			ID,
			GAME_BASED,
			PRIMTIVE,
			MAXBITS
		};
		struct FontUniform
		{
			float col[3];
			unsigned int bufferID;
			Vec2f offset;
			Vec2f scale;

			void SetColor(const float& r, const float& g, const float& b)
			{
				col[0] = r;
				col[1] = g;
				col[2] = b;
			}
		};
		struct BufferID 
		{
			unsigned int dat[MAXBITS];

			BufferID(unsigned int primitive, unsigned int shader, unsigned int worldSpace, unsigned int id = 0)
			{
				dat[SHADER] = shader;
				dat[ID] = id;
				dat[GAME_BASED] = worldSpace;
				dat[PRIMTIVE] = primitive;
			}

			const bool operator<(const BufferID& o) const
			{
				for (unsigned char i{}; i < MAXBITS; ++i)
				{
					if (dat[i] < o.dat[i])
						return true;
					if (dat[i] > o.dat[i])
						return false;
				}
				return false;
			}

			const bool operator==(const BufferID& o) const
			{
				for (unsigned char i{}; i < MAXBITS; ++i)
				{
					if (dat[i] != o.dat[i])
						return false;
				}
				return true;
			}
		};

		std::map<BufferID, BatchBuffer> mBufferMap;//shder << 8 | primitive // use switch(num) case AssetManager::getShder(enConfig::Baisc)
		std::map<unsigned int, FontUniform> mFontUniforms;

		void InitScreenDimension(const float& screenWidth, const float& screenHeight);
		void Render(const unsigned int& cam);

		FontUniform* GetFontUniforms(const unsigned int& bufferID);
		unsigned int AssignFont(const unsigned int& e);
		void UnassignFont(const unsigned int& e);
	};
}

#endif