/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ForcesManager.h

 author:	   Lee Yong Yee(100%)

 email:        l.yongyee@digipen.edu

 brief:        Contains the struct declaration for each type of force and the class declaration of the forces manager as well its public functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifndef FORCES_MANAGER_H
#define FORCES_MANAGER_H

#include "Math/Vec2.h"
#include <vector>



namespace Carmicah
{
    //brief:         This struct contains the properties of linear forces that will applied for each dynamic and kinematic object
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

    //brief:         This struct contains the properties of drag forces for each dynamic and kinematic object
    struct DragForce 
    {
        float directionalDrag;
        float rotationalDrag;

        bool isActive;

        DragForce(float directDrag, float rotDrag)
            : directionalDrag(directDrag), rotationalDrag(rotDrag), isActive(true) {}

        Vector2D<float> CalculateDragForce(const Vector2D<float>& currentVelocity) const;

    };

    //brief:         This class is a manager that handles the forces of each game object
	class ForcesManager
	{
        std::vector<LinearDirectionalForce> linearForces;
        DragForce dragForce;
        
        Vector2D<float> objectVelocity;
        Vector2D<float> accumulatedForce;

        public:

        ForcesManager() : dragForce(0.9f, 0.0f) {}  // Adjust initial values as needed

        void AddLinearForce(LinearDirectionalForce& force);

        void SetCurrentVelocity(const Vector2D<float>& velocity);

        void RemoveForce();

        // Getter for linearForces
        std::vector<LinearDirectionalForce>& GetLinearForces() 
        {
            return linearForces; // Return a const reference to avoid copying
        }

		void UpdateForces(float deltaTime);

        Vector2D<float> GetSumForces() const;

        Vector2D<float> SetSumForces(Vector2D<float> value);

        //void StopObject(float deltaTime);
	};
}

#endif

