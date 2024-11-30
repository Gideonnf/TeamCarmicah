/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			TextGraphicsSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			TextSystem handles the rendering of text to the screen

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef TEXT_SYSTEM_H
#define TEXT_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "Graphics/BaseGraphicsSystem.h"

namespace Carmicah
{
	class TextGraphicsSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		template <typename T>
		void ReplaceTextBuffer(const Entity& e, std::vector<T> cpyDat, size_t startPos = 0, size_t endPos = 0)
		{
			// Gets related data
			EntityData& dat = mEntityBufferLoc.find(e)->second;
			int& mBatchSize = AssetManager::GetInstance()->enConfig.batchRenderSize;
			size_t numVtx = dat.mBufferData->pRef->vtx.size();

			if (cpyDat.size() == 0)
			{
				bool KILLALL{ (endPos | startPos) == 0 };

				// if endPos is null, set end to max
				if (endPos == 0)
					endPos = dat.mBufferData->buffer.size() * mBatchSize;
				for (size_t i{ startPos / mBatchSize }; i < dat.mBufferData->buffer.size(); ++i)
				{
					startPos = startPos % mBatchSize;
					size_t numVtxToChange = numVtx * 
						((endPos < static_cast<size_t>(mBatchSize) ? endPos : static_cast<size_t>(mBatchSize)) - startPos);
					if (numVtxToChange == 0)
						break;

					cpyDat.resize(numVtxToChange);

					glNamedBufferSubData(dat.mBufferData->buffer[i].vbo, sizeof(T) * startPos * numVtx, sizeof(T) * numVtxToChange, cpyDat.data());

					startPos = 0;
					endPos -= numVtxToChange;
				}

				// KILL ALL
				if (KILLALL)
				{
					ToggleActiveEntity(false);
					//dat.mBufferData->freeData = std::queue<unsigned int>(); // font dosen't use this
					dat.mBufferData->objCount = 0;
					// Erase from MAP Not done here
					RenderHelper::GetInstance()->UnassignFont(e); // unassign this Buffer ID back to the queue for other fonts to use
				}
			}
			else
			{
				for (size_t i{ startPos / mBatchSize }; i < dat.mBufferData->buffer.size(); ++i)
				{
					startPos = startPos % mBatchSize;
					size_t numVtxToChange = (cpyDat.size() < static_cast<size_t>(mBatchSize) * numVtx ? cpyDat.size() : static_cast<size_t>(mBatchSize) * numVtx);
					if (numVtxToChange == 0)
						break;
					if (numVtxToChange != 0)
					{
						glNamedBufferSubData(dat.mBufferData->buffer[i].vbo, sizeof(T) * startPos * numVtx, sizeof(T) * numVtxToChange, cpyDat.data());
					}
					startPos = 0;
					cpyDat.erase(cpyDat.begin(), cpyDat.begin() + numVtxToChange - 1);
				}
			}
		}

	public:
		/*!*************************************************************************
		brief
			Sets which components the system references, and load the shader
		***************************************************************************/
		void Init();

		void EntityDestroyed(Entity id);

		/*!*************************************************************************
		brief
			Renders text of objects with text and UITransform Components
		***************************************************************************/
		void Update();
	};
}
#endif