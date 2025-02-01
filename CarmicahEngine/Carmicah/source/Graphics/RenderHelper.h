/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			RenderHelper.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Handles the actual rendering of all entities, and also the batching and instancing of different entities
				Also handles the Rendering of Gizmos when in editor mode

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
		/*!*************************************************************************
		brief
			Renders the gizmos if any onto the FBO
		***************************************************************************/
		void RenderGizmos();

		Mtx3x3f screenMtx{};	// Calculated Screen Matrix multiplier to use for UI rendering

		const float MIN_HEIGHT_SCALE{ 0.01f }, EDITOR_ZOOM_SPEED{ 0.1f }, GIZMO_SCALE{ 250.f };
		// Data storage for managing fonts
		static unsigned int sCapFontID;
		static std::queue<unsigned int> sUnusedFontID;
		std::map<unsigned int, unsigned int> mFontBufferToEntity;
		static float zeroFiller[4];
		static float oneFiller[4];

	public:
		// Buffer details
		enum BUFFER_BITS : unsigned char
		{
			BUFFER_SHADER = 0,
			BUFFER_ID,
			BUFFER_GAME_BASED,
			BUFFER_PRIMTIVE,
			BUFFER_MAXBITS
		};
		struct BufferID 
		{
			unsigned int dat[BUFFER_MAXBITS];

			/*!*************************************************************************
			brief
				Constructor for a Buffer ID
			param[primitive]
				Buffer ID related sorting data
			param[shader]
				Buffer ID related sorting data
			param[worldSpace]
				Buffer ID related sorting data
			param[id]
				Buffer ID related sorting data
			***************************************************************************/
			BufferID(unsigned int primitive, unsigned int shader, unsigned int worldSpace, unsigned int id = 0)
			{
				dat[BUFFER_SHADER] = shader;
				dat[BUFFER_ID] = id;
				dat[BUFFER_GAME_BASED] = worldSpace;
				dat[BUFFER_PRIMTIVE] = primitive;
			}

			/*!*************************************************************************
			brief
				Comparison test for Buffer IDs for sorting
			param[0]
				Buffer ID to compare against
			return
				if this BufferID is less than the other BufferID
			***************************************************************************/
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

			/*!*************************************************************************
			brief
				Comparison test for Buffer IDs for sorting
			param[0]
				Buffer ID to compare against
			return
				if this BufferID is equilviant to the other BufferID
			***************************************************************************/
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
		// Font details
		struct FontUniform
		{
			float col[3];
			unsigned int bufferID;
			Vec2f offset;
			Vec2f scale;

			/*!*************************************************************************
			brief
				Sets the color for the Font uniform
			param[r]
				Red (0.f ~ 1.f)
			param[g]
				Green (0.f ~ 1.f)
			param[b]
				Blue (0.f ~ 1.f)
			***************************************************************************/
			void SetColor(const float& r, const float& g, const float& b)
			{
				col[0] = r;
				col[1] = g;
				col[2] = b;
			}
		};
		// Editor Gizmos specific things
		enum class GIZMOS_MODE
		{
			GIZMOS_NONE,
			GIZMOS_TRANSLATE,
			GIZMOS_SCALE,
			GIZMOS_ROTATE
		};
		
		// Editor mode things
		Vec2d mOldMousePos{};
		Transform mEditorCam{};
		bool mEditorWindowActive{};
		Vec2f mEditorWindomDim{};// Actual dimensions of the editor window that is being viewed, so that gizmos look okay
		unsigned int mSelectedID{};
		GIZMOS_MODE mEditorMode{ GIZMOS_MODE::GIZMOS_NONE };

		std::map<BufferID, BatchBuffer> mBufferMap;//shder << 8 | primitive // use switch(num) case AssetManager::getShder(enConfig::Baisc)
		// Map for the font uniform for the corresponding entity ID
		std::map<unsigned int, FontUniform> mFontUniforms;

		// General

		/*!*************************************************************************
		brief
			Uses the provided screen width and height to initialize the UI dimensions,
			and to set the Editor camera scale to follow a ratio
		param[screenWidth]
			screen width
		param[screenHeight]
			screen height
		***************************************************************************/
		void InitScreenDimension(const float& screenWidth, const float& screenHeight);

		/*!*************************************************************************
		brief
			Uses inputs to update the editor camera position and scale
			Move	--->	Mouse Right click drag
			Scale	--->	+ / - ke
		***************************************************************************/
		void UpdateEditorCam();

		/*!*************************************************************************
		brief
			Tries to render the screen with or with a camera component
				will just render UI things if no camera provided
		param[cam]
			optional camera to render based off on
		param[isEditor]
			bool to check to render debug things -> Collider, direction vector, Gizmos 
		***************************************************************************/
		void Render(std::optional<Transform*> cam, bool isEditor = false);

		/*!*************************************************************************
		brief
			Overload of the Render function
		param[cam]
			entity to try and find a camera component from
		param[isEditor]
			bool to check to render debug things -> Collider, direction vector, Gizmos
		***************************************************************************/
		void Render(const unsigned int& cam, bool isEditor = false);

		// Fonts
		/*!*************************************************************************
		brief
			Gets the uniforms needed to render the specific font (for editing)
		param[bufferID]
			the id to find the uniforms from
		return
			pointer to the font uniform found
		***************************************************************************/
		FontUniform* GetFontUniforms(const unsigned int& bufferID);

		/*!*************************************************************************
		brief
			Handles the data allocation for fonts uniforms and buffers
		param[e]
			the entity id for the font game object
		return
			the unique id for the buffer that the font uses
		***************************************************************************/
		unsigned int AssignFont(const unsigned int& e);

		/*!*************************************************************************
		brief
			Releases the font uniform and buffer data back to the RenderHelper to manage when another font game object is created
			-(to recycle instead of trying to delete the buffer data immediately)
		param[e]
			the entity id for the font game object
		***************************************************************************/
		void UnassignFont(const unsigned int& e);

		/*!*************************************************************************
		brief
			Loads the GLModels needed for rendering Gizmos
		***************************************************************************/
		void LoadGizmos();
	};
}

#endif