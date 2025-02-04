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
			unsigned int id{};
		};

		// Data store for finding out the bufferLocation which is linked to the entity of Primitive / BasePrimitive Type
		struct EntityData
		{
			BatchBuffer* mBufferData{};
			unsigned int posInMemory{};
		};
#pragma endregion

		GLuint mCurrShader{}, mActiveEntityCount{};
		float mFurtherstDepth{}, mNearestDepth{};
		std::unordered_map<unsigned int, EntityData> mEntityBufferLoc{}; // copy of mEntitySet for use of tracking valid entities
		/*!*************************************************************************
		brief
			Inits basic data for the different graphics systems
		param[shdr]
			name of shader to use
		***************************************************************************/
		virtual void Init(const std::string& shdr);


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
			Appends a new entity's data into the buffer (makes new buffer if none exists)
		param[entity]
			the entity ID
		param[primitiveName]
			the name of the primitive to link the buffer to
		param[id]
			unique id of the buffer (other than the other identifiers)
		param[worldSpace]
			to separate the batch buffers based on the need to use a camera as a uniform
		param[isDebug]
			to know to use vtx2D or vtxTexd2D for storing data into the buffer
		***************************************************************************/
		void SetNewEntity(const unsigned int& entity, const std::string& primitiveName, const unsigned int& id, bool worldSpace, bool isDebug);

		/*!*************************************************************************
		brief
			Generates a Vertex Buffer and accompanying data to fill mBatchSize number of Primitives(with tex)/ BasePrimitives(no tex)
			** Does not generates a new batch if current batch is filled **
			** Not meant for objects with transparency **
		param[primitiveName]
			the primitive data to use to generate Batch Buffer Object & details
		param[id]
			unique id of the buffer (other than the other identifiers)
		param[worldSpace]
			to separate the batch buffers based on the need to use a camera as a uniform
		param[isDebug]
			toggle to generate a new batch of rendering for BasePrimitive objects
		param[isForced]
			force generate a new batch of primitives, reduce checks
		***************************************************************************/
		void GenBatch(const std::string& primitiveName, unsigned int id, bool worldSpace, bool isDebug, bool isForced = false);

		/*!*************************************************************************
		brief
			Edits the data for a specific Entity located inside the Batch Buffer Data
		param[entity]
			ID of the entity whose data to pass into the Buffer Object
		param[worldSpace]
			Boolean value for finding the Transform / UITransform data of the entity
		param[layer]
			The layer that the object should be rendered at, so that depth can be properly calculated relative to other objects
		***************************************************************************/
		void EditBatchData(const unsigned int& entity, bool worldSpace, RENDER_LAYERS layer);

		/*!*************************************************************************
		brief
			Edits the data for a specific Entity with Debug Primitives located inside the Batch Buffer Data
		param[entity]
			ID of the entity whose data to pass into the Buffer Object
		param[mtx]
			calculated matrix to use for multiplication against the vertex of the BasePrimitive
		param[worldSpace]
			Boolean value for finding the Transform / UITransform data of the entity
		param[layer]
			The layer that the object should be rendered at, so that depth can be properly calculated relative to other objects
		***************************************************************************/
		void EditDebugBatchData(const unsigned int& entity, Mtx3x3f& mtx, bool worldSpace, RENDER_LAYERS layer);

		/*!*************************************************************************
		brief
			Clears up the specific Buffer data at the location
		param[entity]
			ID of the entity whose data to pass into the Buffer Object
		param[eraseFromMap]
			to remove from the map in this function
		***************************************************************************/
		void DeleteBatchData(const unsigned int& entity, bool eraseFromMap = false);

		/*!*************************************************************************
		brief
			Getter and Setter for Toggling Entity Data
		param[setActive]
			Sets that position in buffer to be active or deactive
		***************************************************************************/
		void ToggleActiveEntity(bool setActive);

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