#pragma once

#include "vector3.h"

namespace rasterizer::math
{
    struct Transform
    {
        float scale;
        float rotation;
        Vector3 translation;

        Transform() : scale(1.0f), rotation(0.0f), translation(0.0f, 0.0f, 0.0f) {}
        Transform(float scale, float rotation, Vector3 translation)
            : scale(scale), rotation(rotation), translation(translation) {}
    };
}