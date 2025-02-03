/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Transform.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of the transform data in worldspace

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Math/Matrix3x3.h"
#include "BaseTransform.h"

namespace Carmicah
{
    // Use bitshift to get a unique bit number for each layer
    // Use 32-bit unsigned int for the layers for now
    // i dont think we'll ever get more than that
    // If we want to make this changable in the editor
    // we could create an array of 32 uint_32t that keep track of the bit for each layer
    // so that the editor can create and remove more layers but we just hard set the layers for now
    enum class CollisionLayer : uint32_t
    {
        DEFAULT = 1 << 0, // 0000 .... 0001
        PLAYER = 1 << 1, // 0000 .... 0010
        ENEMIES = 1 << 2, // 0100
        ENVIRONMENT = 1 << 3, // 1000
        TOTAL_LAYERS = 1 << 4
    };

    struct Transform : BaseTransform<Transform>
    {
        unsigned int collisionMask = (unsigned int)CollisionLayer::DEFAULT;
        Matrix3x3<float> worldSpace;
        Matrix3x3<float> rotTrans;
        // So there's 3 scales
        // Scale()          -> the scale you see in the editor
        // InternalScale    -> the scale needed for graphics calc
        // accumulatedScale -> the scale that is the combination of parents and itself
        Vector2D<float> accumulatedScale = Vector2D<float>::one();

        Transform& DeserializeComponent(const rapidjson::Value& component) override
        {           
            BaseTransform::DeserializeComponentBuffer(component);
            if (component.HasMember("collisionMask"))
            {
                collisionMask = static_cast<unsigned int>(component["collisionMask"].GetUint());
            }
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            BaseTransform::SerializeComponent(writer);
            writer.String("collisionMask");
            writer.Uint(collisionMask);
        }
    };
}

#endif