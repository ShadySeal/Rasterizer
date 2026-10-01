#pragma once

#include "math/vector3.h"

namespace rasterizer::rendering
{
    struct Plane
    {
        rasterizer::math::Vector3 normal;
        float d;

        static std::vector<Plane> makeFrustumPlanes()
        {
            const float invSqrt2 = 1.0f / std::sqrt(2.0f);
            const float nearD = 1.0f;
            const float farD  = 100.0f;

            return {
                { rasterizer::math::Vector3( invSqrt2, 0, invSqrt2), 0.0f },  // left
                { rasterizer::math::Vector3(-invSqrt2, 0, invSqrt2), 0.0f },  // right
                { rasterizer::math::Vector3(0, -invSqrt2, invSqrt2), 0.0f },  // top
                { rasterizer::math::Vector3(0,  invSqrt2, invSqrt2), 0.0f },  // bottom
                { rasterizer::math::Vector3(0, 0, 1), -nearD },                // near
                { rasterizer::math::Vector3(0, 0, -1), farD }                  // far
            };
        }
    };
}