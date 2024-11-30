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
#include <optional>
#include "Singleton.h"
#include "Systems/AssetTypes.h"
#include "Components/BaseComponent.h"
#include "Components/Transform.h"

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
		
		const float minHeightScale{ 0.01f };
		static unsigned int mCapFontID;
		static std::queue<unsigned int> mUnusedFontID;
		std::map<unsigned int, unsigned int> mFontBufferToEntity;

	public:
		enum BUFFER_BITS : unsigned char
		{
			BUFFER_SHADER = 0,
			BUFFER_ID,
			BUFFER_GAME_BASED,
			BUFFER_PRIMTIVE,
			BUFFER_MAXBITS
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
			unsigned int dat[BUFFER_MAXBITS];

			BufferID(unsigned int primitive, unsigned int shader, unsigned int worldSpace, unsigned int id = 0)
			{
				dat[BUFFER_SHADER] = shader;
				dat[BUFFER_ID] = id;
				dat[BUFFER_GAME_BASED] = worldSpace;
				dat[BUFFER_PRIMTIVE] = primitive;
			}

			const bool operator<(const BufferID& o) const
			{
				for (unsigned char i{}; i < BUFFER_MAXBITS; ++i)
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
				for (unsigned char i{}; i < BUFFER_MAXBITS; ++i)
				{
					if (dat[i] != o.dat[i])
						return false;
				}
				return true;
			}
		};
		enum class GIZMOS_MODE
		{
			GIZMOS_NONE,
			GIZMOS_TRANSLATE,
			GIZMOS_SCALE,
			GIZMOS_ROTATE
		};
		

		Vec2d mOldMousePos{};
		Transform mEditorCam{};
		bool mEditorWindowActive{};
		Vec2f mEditorWindomDim{};// Actual dimensions of the editor window that is being viewed, so that gizmos look okay
		unsigned int mSelectedID{};
		GIZMOS_MODE mEditorMode{ GIZMOS_MODE::GIZMOS_NONE };

		std::map<BufferID, BatchBuffer> mBufferMap;//shder << 8 | primitive // use switch(num) case AssetManager::getShder(enConfig::Baisc)
		std::map<unsigned int, FontUniform> mFontUniforms;

		// General
		void InitScreenDimension(const float& screenWidth, const float& screenHeight);
		void UpdateEditorCam();
		void Render(std::optional<Transform*> cam, bool isEditor = false);
		void Render(const unsigned int& cam, bool isEditor = false);

		// Fonts
		FontUniform* GetFontUniforms(const unsigned int& bufferID);
		unsigned int AssignFont(const unsigned int& e);
		void UnassignFont(const unsigned int& e);

		void LoadGizmos();
	private:
		const float mGizmoScale{250.f};
		void RenderGizmos();
	};
}

#endif