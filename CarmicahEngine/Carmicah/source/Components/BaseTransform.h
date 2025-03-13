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
#include "ECS/ECSTypes.h"
#include "Math/Vec2.h"

namespace Carmicah
{
    template <typename T>
    struct BaseTransform : BaseComponent<T>
    {
    protected:
        Vec2f pos{};
        Vec2f scale{};
        Vec2f intScale{};// InternalScale

        float rot;
        float depth{};
        Entity parent{}; // Hold 0 if no parent
        unsigned int grandChildLvl{};

        bool notUpdated{};

    public:
        std::string transformTag;
        std::vector<Entity> children;

        Matrix3x3<float> worldSpace;
        Matrix3x3<float> rotTrans;
        // So there's 3 scales
        // Scale()          -> the scale you see in the editor
        // InternalScale    -> the scale needed for graphics calc
        // accumulatedScale -> the scale that is the combination of parents and itself
        Vector2D<float> accumulatedScale = Vector2D<float>::one();

#pragma region Getter Setters
        Vec2f ExtractWorldPos()
        {
            return Vec2f(rotTrans.m[6], rotTrans.m[7]);
        }

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
        void InternalScale(const float& x, const float& y)
        {
            intScale.x = x;
            intScale.y = y;
            notUpdated = false;
        }
        Vec2f CalcedRenderingScale()
        {
            return intScale + Vec2f::one();
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
        void RotAdd(const float& rhs)
        {
            Rot(rot + rhs);
        }
        void SetParent(const Entity& parentID, const unsigned int& parentGrandChildLevel)
        {
            parent = parentID;
            if (parentID == 0)
                grandChildLvl = 0;
            else
                grandChildLvl = parentGrandChildLevel + 1;
        }
        const Entity& ParentID()
        {
            return parent;
        }
        const unsigned int& GrandChildLevel()
        {
            return grandChildLvl;
        }
        bool Updated()
        {
            return !notUpdated;
        }
        void Update()
        {
            notUpdated = false;
        }
        void ResetUpdate()
        {
            notUpdated = true;
        }
#pragma endregion


        void DeserializeComponentBuffer(const rapidjson::Value& component)
        {
            pos.x = static_cast<float>(component["xPos"].GetDouble());
            pos.y = static_cast<float>(component["yPos"].GetDouble());
            depth = static_cast<float>(component["depth"].GetDouble());
            scale.x = static_cast<float>(component["xScale"].GetDouble());
            scale.y = static_cast<float>(component["yScale"].GetDouble());
            rot = static_cast<float>(component["rot"].GetDouble());
            notUpdated = false;
            DESERIALIZE_IF_HAVE(transformTag, component, "transformTag", GetString, std::string);
            
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
            writer.String("rot");
            writer.Double(rot);
            writer.String("transformTag");
            writer.String(transformTag.c_str());

        }
    };
}


#endif