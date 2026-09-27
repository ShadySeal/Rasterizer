#pragma once

#include "math/vector3.h"

namespace rasterizer::rendering
{
    struct Plane
    {
        rasterizer::math::Vector3 normal;
        float d;
    };
}