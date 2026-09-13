#pragma once

#include "color.h"

using namespace rasterizer::math;

namespace rasterizer::rendering
{
    struct Triangle
    {
        int v[3];
        Color color;

        Triangle(int v1, int v2, int v3, Color color)
            : v{v1, v2, v3}, color(color) {};
    };
}