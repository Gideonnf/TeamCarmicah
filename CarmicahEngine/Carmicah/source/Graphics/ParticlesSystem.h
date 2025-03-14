/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			ParticlesSystem.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Graphics System handles the rendering of textured meshes onto the screen (using transform, camera reference, and texture matrix)

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef PARTICLES_SYSTEM_H
#define PARTICLES_SYSTEM_H

#include "ECS/BaseSystem.h"
#include "Graphics/BaseGraphicsSystem.h"

namespace Carmicah
{
	class ParticlesSystem : public BaseSystem, private BaseGraphicsSystem
	{
	private:
		struct particle
		{
			std::string texture;
			Mtx3x3f mtx;
			Vec2f vel,			// Vel Change per second
				alpha;			// Curr Alpha, Alpha change per second
			float depth;
			// --TODO--
			//float scaling,	// uses a (y = a^x) when x = 1, y ~= 1, z ?= 0.01, so just use this and multiply by how long
			float	timeLeft;
			bool hasFriction;
			bool hasGrav;
		};

		// 0 is World, 1 is UI
		std::vector<particle> mParticles[2];		// Actual Particle data

		const float mGrav = 9.8f;
		const float mFriction = 0.99f;
	public:
		/*!*************************************************************************
		brief
			Sets which components the system references, and load the shader
		***************************************************************************/
		void Init();

		/*!*************************************************************************
		brief
			Updates the entities position in buffer
		***************************************************************************/
		void Update();

	};
}

#endif