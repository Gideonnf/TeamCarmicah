/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			GraphicsAssetTypes.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Handles the actual rendering of all entities, and also the batching and instancing of different entities
				Also handles the Rendering of Gizmos when in editor mode

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef GRAPHICS_ASSET_TYPES_H
#define GRAPHICS_ASSET_TYPES_H
#include "Systems/AssetTypes.h"
#include "Math/Vec2.h"
#include "Math/Matrix3x3.h"

namespace Carmicah
{
	// Rendering layers
	enum RENDER_LAYERS
	{
		BASE_LAYER = 0,
		DEBUG_LAYER,
		UI_LAYER,
		DEBUG_UI_LAYER,
		MAX_RENDER_LAYERS
	};
	// Data stored in this order used for instancing of basic / fonts shader with textures
	struct vtxTexd2D
	{
		Vec2f pos{};
		Vec2f uv{};
		float color[4]{};
		float depth{};
		unsigned int ids[2]{};// Entity id,  Tex id
	};
	// Data stored in this order used for instancing of debug shader
	struct vtx2D
	{
		Vec2f pos{};
		float depth{};
		unsigned int id{};
	};
	// Batch Rendering details
	struct DrawElemCmd
	{
		GLuint vtxCnt{}, instanceCnt{}, firstIdx{}, baseVtx{}, baseInstance{};
	};
	// Data store for finding out the bufferLocation which is linked to the entity of Primitive / BasePrimitive Type
	struct EntityData
	{
		BatchBuffer* mBufferData{};
		unsigned int posInMemory{};
	};

	// Buffer details
	enum BUFFER_BITS : unsigned char
	{
		BUFFER_SHADER = 0,
		BUFFER_ID,			// ID Means Check CPU Buffer
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
	struct BufferCPUSide
	{
		std::vector<vtxTexd2D> vtx;
		Vec2f offset;
		Vec2f scale;
		float col[3];
		int vtxSize = 0; // because vtx getting resized in 100 batches * 4 vtxs, dun keep actl size there
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

}
#endif