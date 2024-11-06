/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        PhysicsSystem.h

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Contains the Class declaration of the Physics System as well its public functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifndef FORCES_MANAGER_H
#define FORCES_MANAGER_H

#include "Math/Vec2.h"
#include <vector>


//berif:         This class is a system that handles the physics of all the game objects
namespace Carmicah
{
    struct LinearDirectionalForce
    {
        Vector2D<float> unitDirection; //What direction object is going in
        float magnitude; //How strong the force is
        float lifetime;
        float age = 0.0f;
        bool isActive = true;
        
        LinearDirectionalForce(Vector2D<float> direction, float mag, float life)
            : unitDirection(direction), magnitude(mag), lifetime(life) {}

        Vector2D<float> GetForceVector() const;

        float SetLifeTime(float lifeTimeValue);

        void Update(float deltaTime);

    };

    struct DragForce 
    {
        float directionalDrag;
        float rotationalDrag;

        float lifetime;
        bool isActive = true;

        DragForce(float directDrag, float rotDrag, float life)
            : directionalDrag(directDrag), rotationalDrag(rotDrag), lifetime(life) {}

        //void Update(float)
    };

	class ForcesManager
	{
        std::vector<LinearDirectionalForce> linearForces;
        std::vector<LinearDirectionalForce> dragForces;

        Vector2D<float> accumulatedForce;

        public:
        void AddLinearForce(LinearDirectionalForce& force);

        void RemoveLinearForce();

        // Getter for linearForces
        std::vector<LinearDirectionalForce>& GetLinearForces() 
        {
            return linearForces; // Return a const reference to avoid copying
        }

		void UpdateForces(float deltaTime);

        Vector2D<float> GetSumForces();

        Vector2D<float> SetSumForces(Vector2D<float> value);
		//void ApplyForces(Entity& obj, Vector2D<float>& velocity);
	};
}

#endif

