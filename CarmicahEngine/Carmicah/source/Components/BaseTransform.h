/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			BaseTransform.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of transform data shared by both UI Objects, and WorldSpace Objects

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef BASE_TRANSFORM_H
#define BASE_TRANSFORM_H

#include "BaseComponent.h"
#include "Math/Vec2.h"

namespace Carmicah
{
    template <typename T>
    struct BaseTransform : BaseComponent<T>
    {
        Vec2f pos;
        Vec2f scale;
        float depth;

        bool notUpdated;

        Entity parent; // Hold 0 if no parent
        std::vector<Entity> children;

<<<<<<< Updated upstream
=======
#pragma region Getter Setters
        const Vec2f& Pos() const
        {
            return pos;
        }
        Vec2f& GetPos()
        {
            notUpdated = false;
            return pos;
        }
        void Pos(const Vec2f& rhs)
        {
            Pos(rhs.x, rhs.y);
        }
        void PosX(const float& rhs)
        {
            Pos(rhs, pos.y);
        }
        void PosXAdd(const float& rhs)
        {
            Pos(pos.x + rhs, pos.y);
        }
        void PosY(const float& rhs)
        {
            Pos(pos.x, rhs);
        }
        void PosYAdd(const float& rhs)
        {
            Pos(pos.x, pos.y + rhs);
        }
        void Pos(const float& x, const float& y)
        {
            pos.x = x;
            pos.y = y;
            notUpdated = false;
        }
        const Vec2f& Scale() const
        {
            return scale;
        }
        Vec2f& GetScale()
        {
            notUpdated = false;
            return scale;
        }
        void Scale(const Vec2f& rhs)
        {
            Scale(rhs.x, rhs.y);
        }
        void ScaleX(const float& rhs)
        {
            Scale(rhs, scale.y);
        }
        void ScaleXAdd(const float& rhs)
        {
            Scale(scale.x + rhs, scale.y);
        }
        void ScaleY(const float& rhs)
        {
            Scale(scale.x, rhs);
        }
        void ScaleYAdd(const float& rhs)
        {
            Scale(scale.x, scale.y + rhs);
        }
        void Scale(const float& x, const float& y)
        {
            scale.x = x;
            scale.y = y;
            notUpdated = false;
        }
        
        const float& Depth() const
        {
            return depth;
        }
        float& GetDepth()
        {
            notUpdated = false;
            return depth;
        }
        void Depth(float d)
        {
            depth = d;
            notUpdated = false;
        }
        const float& Rot() const
        {
            return rot;
        }
        float& GetRot()
        {
            notUpdated = false;
            return rot;
        }
        void Rot(float d)
        {
            rot = d;
            notUpdated = false;
        }

        const bool& Updated() const
        {
            return !notUpdated;
        }

        void ResetUpdate()
        {
            notUpdated = true;
        }
#pragma endregion

>>>>>>> Stashed changes

        void DeserializeComponentBuffer(const rapidjson::Value& component)
        {
            pos.x = static_cast<float>(component["xPos"].GetDouble());
            pos.y = static_cast<float>(component["yPos"].GetDouble());
            depth = static_cast<float>(component["depth"].GetDouble());
            scale.x = static_cast<float>(component["xScale"].GetDouble());
            scale.y = static_cast<float>(component["yScale"].GetDouble());
        }

        virtual void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("xPos");
            writer.Double(pos.x);
            writer.String("yPos");
            writer.Double(pos.y);
            writer.String("depth");
            writer.Double(depth);
            writer.String("xScale");
            writer.Double(scale.x);
            writer.String("yScale");
            writer.Double(scale.y);

        }
    };
}


#endif