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
#include "GraphicsAssetTypes.h"

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
		// Data for clearing FBO texture data
		static float zeroFiller[4];
		static float oneFiller[4];

	public:
		
		// Editor mode things
		Vec2d mOldMousePos{};
		Transform mEditorCam{};
		bool mEditorWindowActive{};
		Vec2f mEditorWindomDim{};// Actual dimensions of the editor window that is being viewed, so that gizmos look okay
		unsigned int mSelectedID{};
		GIZMOS_MODE mEditorMode{ GIZMOS_MODE::GIZMOS_NONE };

		std::map<BufferID, BatchBuffer> mBufferMap;//shder << 8 | primitive // use switch(num) case AssetManager::getShder(enConfig::Baisc)

		std::map<unsigned int, BufferCPUSide> mFontData;
		BufferCPUSide mParticleData[2];

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

		/*!*************************************************************************
		brief
			Final rendering of the FBO onto the screen
		***************************************************************************/
		void FinalRender();

		// Fonts
		/*!*************************************************************************
		brief
			Handles the data allocation for fonts uniforms and buffers
		param[e]
			the entity id for the font game object
		return
			the buffer that the font uses
		***************************************************************************/
		void AssignFont(const unsigned int& e);

		/*!*************************************************************************
		brief
			Handles the Assignment and Checking of the correct size of the buffer allocated so far
		param[e]
			the entity id for the font game object
		return
			the buffer that the font uses
		***************************************************************************/
		void ReserveFontBuffer(BufferCPUSide& buff, const size_t& sz);

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