#pragma once

#include "math/vector3.h"

namespace rasterizer::rendering
{
    struct BoundingSphere
    {
        rasterizer::math::Vector3 center;
        float radius;
    };
}