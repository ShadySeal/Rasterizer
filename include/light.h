#pragma once

#include <algorithm>
#include "vector3.h"

namespace rasterizer
{
    struct Light
    {
        enum Type
        {
            AMBIENT,
            POINT,
            DIRECTIONAL
        };

        Type type;
        float intensity;
        Vector3 position;
        Vector3 direction;

        Light(Type type, float intensity, Vector3 position = Vector3(0, 0, 0), Vector3 direction = Vector3(0, 0, 0))
            : type(type), intensity(intensity), position(position), direction(direction) {}
    };
}