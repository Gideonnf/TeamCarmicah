/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:         CollisionSystem.h

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Contains the Class declaration of the Collision System as well its public functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include "ECS/BaseSystem.h"
#include "../Math/Vec2.h"
#include <bitset>
#include <unordered_set>

namespace Carmicah
{

	class CollisionSystem : public BaseSystem
	{
	private:

		static constexpr int MAX_ENTITIES = 1024;
		static constexpr int GRID_WIDTH = 100;
		static constexpr int GRID_HEIGHT = 100;
		float cellSize = 3.5f;

		std::bitset<MAX_ENTITIES> rowsBitArray[GRID_HEIGHT];
		std::bitset<MAX_ENTITIES> colsBitArray[GRID_WIDTH];

		std::unordered_map<Entity, int> entityIndexMap;
		int entityCounter = 0;

		std::unordered_map<Entity, std::unordered_set<Entity>> mEntityTriggerMap;

		
	public:
		void PrintEntities();

		void ClearGrid();

		void InsertEntityToGrid(Entity& entity, const Vec2f& position);

		int GetEntityIndex(Entity& entity);

		std::vector<Entity> GetPotentialCollisions(Entity& entity, const Vec2f& position);

		void UpdateOBB(Entity& obj);

		void ComputeProjInterval(Entity& obj, Vec2f edgeNormal, float& min, float& max);

		void GetOBBVertices(Entity& obj);

		void CalculateEdges(Entity& obj);

		float CalculatePenetrationDepth(Entity& obj1, Entity& obj2);

		Vec2f CalculateCollisionNormal(Entity& obj1, Entity& obj2);

		void ResolvePenetration(Entity& obj1, Entity& obj2);
		
		int WhichSide(std::vector<Vec2f>& otherVertices, Vec2f& point, Vec2f& normal);

		bool TestIntersection(Entity& obj1, Entity& obj2);

		void CollisionResponse(Entity& obj1, Entity& obj2);

		void CollisionCheck();

		bool CollisionCheck(Entity id, Vec2f displacement);

		void Init();

		void Update();

		void Exit();
	};
}

