/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			BaseGraphicsSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Similar function used across all rendering systems to help in rendering

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef BASE_GRAPHICS_SYSTEM
#define BASE_GRAPHICS_SYSTEM

#include "Systems/AssetTypes.h"
#include "Math/Vec2.h"
#include "Math/Matrix3x3.h"

namespace Carmicah
{
	class BaseGraphicsSystem
	{
	protected:
#pragma region Structs
		// Rendering layers
		enum RENDER_LAYERS
		{
			BASE_LAYER = 0,
			DEBUG_LAYER,
			UI_LAYER,
			DEBUG_UI_LAYER,
			MAX_LAYERS
		};

		// Batch Rendering details
		struct DrawElemCmd
		{
			GLuint vtxCnt{}, instanceCnt{}, firstIdx{}, baseVtx{}, baseInstance{};
		};

		// Data stored in this order used for instancing of basic / fonts shader with textures
		struct vtxTexd2D
		{
			Vec2f pos{};
			Vec2f uv{};
			float depth{};
			unsigned int ids[2]{};// Entity id,  Tex id
		};

		// Data stored in this order used for instancing of debug shader
		struct vtx2D
		{
			Vec2f pos{};
			float depth{};
		};

		// Data store for finding out the bufferLocation which is linked to the entity of Primitive / BasePrimitive Type
		struct EntityData
		{
			bool isActive{};
			unsigned int posInMemory{};
		};
#pragma endregion

		GLuint mCurrShader{}, mBatchSize{}, mEntityBufferIDTrack{};
		float mFurtherstDepth{}, mNearestDepth{};
		std::vector<BatchBuffer> mBufferData{};
		std::unordered_map<unsigned int, EntityData> mEntityBufferLoc{};

		virtual void Init(std::string shdr);

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
			Calculates the depth to pass into the shaders based on layering and depth within the max and min depth of that layer
		param[depth]
			the local depth value of the object
		param[layer]
			the relative render layer of the object
		return
			the new calculated depth
		***************************************************************************/
		float CalcDepth(const float& depth, const RENDER_LAYERS& layer);

		/*!*************************************************************************
		brief
			Generates a Vertex Buffer and accompanying data to fill mBatchSize number of Primitives (with textures)
		param[p]
			the primitive data to use to generate Batch Buffer Object & details
		***************************************************************************/
		void GenBatch(const Primitive& p);

		/*!*************************************************************************
		brief
			Generates a Vertex Buffer and accompanying data to fill mBatchSize number of BasePrimitives (no textures)
		param[p]
			the primitive data to use to generate Batch Buffer Object & details
		***************************************************************************/
		void GenDebugBatch(const BasePrimitive& p);

		/*!*************************************************************************
		brief
			Edits the data for a specific Entity located inside the Batch Buffer Data
		param[entity]
			ID of the entity whose data to pass into the Buffer Object
		param[pos]
			Location (ID) of where in the Buffer Object to edit
		param[worldSpace]
			Boolean value for finding the Transform / UITransform data of the entity
		param[layer]
			The layer that the object should be rendered at, so that depth can be properly calculated relative to other objects
		***************************************************************************/
		void EditBatchData(const unsigned int& entity, const unsigned int& pos, bool worldSpace, RENDER_LAYERS layer);

		/*!*************************************************************************
		brief
			Edits the data for a specific Entity with Debug Primitives located inside the Batch Buffer Data
		param[entity]
			ID of the entity whose data to pass into the Buffer Object
		param[pos]
			Location (ID) of where in the Buffer Object to edit
		param[primitive]
			BasePrimitive data to use for editing the Buffer Data
		param[mtx]
			calculated matrix to use for multiplication against the vertex of the BasePrimitive
		param[worldSpace]
			Boolean value for finding the Transform / UITransform data of the entity
		param[layer]
			The layer that the object should be rendered at, so that depth can be properly calculated relative to other objects
		***************************************************************************/
		void EditDebugBatchData(const unsigned int& entity, const unsigned int& pos, const BasePrimitive& primitive, Mtx3x3f& mtx, bool worldSpace, RENDER_LAYERS layer);

		/*!*************************************************************************
		brief
			Clears up the specific Buffer data at the location
		param[entity]
			ID of the entity whose data to pass into the Buffer Object
		param[pos]
			Location (ID) of where in the Buffer Object to edit
		param[isDebug]
			To delete from vtx2D or vtxTexd2D depending on debug delete or not
		param[vtxSize]
			number of vertices to delete
		***************************************************************************/
		void DeleteBatchData(const unsigned int& entity, const unsigned int& pos, bool isDebug, int vtxSize);

		/*!*************************************************************************
		brief
			Batch Render mBatchSize number of Primitives, using indexed rendering, textures & GL_TRIANGLES
		***************************************************************************/
		void BatchRender();

		/*!*************************************************************************
		brief
			Batch Render mBatchSize number of BasicPrimitives, using indexed rendering & GL_LINE_LOOP
		***************************************************************************/
		void BatchDebugRender();

		/*!*************************************************************************


		brief	---------------------------- Deprecated ----------------------------
			Renders the primitive one by one using the vao, vbo of the primitive
		param[p]
			The primitive data to render
		***************************************************************************/
		//void RenderPrimitive(const Primitive& p);
	};
}
#endif