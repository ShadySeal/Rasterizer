#pragma once

#include <algorithm>
#include "math/vector3.h"

namespace rasterizer::scene
{
    class Light
    {
    private:
        float intensity;

    public:
        enum Type
        {
            AMBIENT,
            POINT,
            DIRECTIONAL
        };

        Type type;
        rasterizer::math::Vector3 position;
        rasterizer::math::Vector3 direction;

        Light(Type type, float intensity, rasterizer::math::Vector3 position = rasterizer::math::Vector3(0, 0, 0), rasterizer::math::Vector3 direction = rasterizer::math::Vector3(0, 0, 0))
            : type(type), intensity(intensity), position(position), direction(direction) {}

        void setIntensity(float value)
        {
            intensity = std::clamp(value, 0.0f, 1.0f);
        }

        float getIntensity() const
        {
            return intensity;
        }
    };
}