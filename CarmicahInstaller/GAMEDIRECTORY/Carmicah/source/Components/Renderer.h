/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Renderer.h

 author:		Won Yu Xuan Rainne(100%)

 email:			won.m@digipen.edu

 brief:			Keeps track of Rendering specific details such as the model being used, texture, and a mat3 for accessing parts of the texture

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include <string>
#include "BaseComponent.h"
#include "Math/Vec2.h"

namespace Carmicah
{
    struct Renderer : BaseComponent<Renderer>
    {
    protected:
        std::string texture;
        float col[4];
        bool notUpdated;

    public:
        std::string model;

        Renderer() : texture("Default"), model("Square"), col(), notUpdated(false)
        {
            col[0] = col[1] = col[2] = col[3] = 1.f;
        }

        const std::string& Texture() const
        {
            return texture;
        }
        std::string& GetTexture()
        {
            notUpdated = false;
            return texture;
        }
        void Texture(std::string o)
        {
            texture = o;
            notUpdated = false;
        }
        // I'm sorry, there's no vec4 lol
        void Color(float* _in) const
        {
            std::memcpy(_in, col, sizeof(float) * 4);
        }
        void Color(float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f)
        {
            col[0] = r;
            col[1] = g;
            col[2] = b;
            col[3] = a;
            notUpdated = false;
        }
        void SetR(float val)
        {
            col[0] = val;
            notUpdated = false;
        }
        void SetG(float val)
        {
            col[1] = val;
            notUpdated = false;
        }
        void SetB(float val)
        {
            col[2] = val;
            notUpdated = false;
        }
        void SetA(float val)
        {
            col[3] = val;
            notUpdated = false;
        }
        const float& R() const
        {
            return col[0];
        }
        const float& G() const
        {
            return col[1];
        }
        const float& B() const
        {
            return col[2];
        }
        const float& A() const
        {
            return col[3];
        }

        float& GetR()
        {
            notUpdated = false;
            return col[0];
        }
        float& GetG()
        {
            notUpdated = false;
            return col[1];
        }
        float& GetB()
        {
            notUpdated = false;
            return col[2];
        }
        float& GetA()
        {
            notUpdated = false;
            return col[3];
        }

        bool Updated()
        {
            bool ret = notUpdated;
            notUpdated = true;
            return !ret;
        }

        Renderer& DeserializeComponent(const rapidjson::Value& component) override
        {
            model = component["model"].GetString();
            texture = component["texture"].GetString();
            // Added inbetween M3 & M4
            if (component.HasMember("colR"))
            {
                col[0] = static_cast<float>(component["colR"].GetDouble());
                col[1] = static_cast<float>(component["colG"].GetDouble());
                col[2] = static_cast<float>(component["colB"].GetDouble());
                col[3] = static_cast<float>(component["colA"].GetDouble());
            }
            return *this;
        }

        void SerializeComponent(rapidjson::PrettyWriter<rapidjson::OStreamWrapper>& writer) override
        {
            writer.String("model");
            writer.String(model.c_str());
            writer.String("texture");
            writer.String(texture.c_str());
            writer.String("colR");
            writer.Double(static_cast<double>(col[0]));
            writer.String("colG");
            writer.Double(static_cast<double>(col[1]));
            writer.String("colB");
            writer.Double(static_cast<double>(col[2]));
            writer.String("colA");
            writer.Double(static_cast<double>(col[3]));
        }
    };
}

#endif