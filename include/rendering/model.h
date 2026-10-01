#pragma once

#include <string>
#include <vector>
#include "math/vector3.h"
#include "triangle.h"
#include "boundingSphere.h"

namespace rasterizer::rendering
{
    struct Model
    {
        std::string name;
        std::vector<rasterizer::math::Vector3> vertices;
        std::vector<Triangle> triangles;
        BoundingSphere boundingSphere;

        Model(std::string name, std::vector<rasterizer::math::Vector3> vertices, std::vector<Triangle> triangles)
            : name(name), vertices(vertices), triangles(triangles) 
        {
            boundingSphere = computeBoundingSphere(vertices);
        };

        BoundingSphere computeBoundingSphere(const std::vector<rasterizer::math::Vector3>& vertices)
        {
            if (vertices.empty()) return { rasterizer::math::Vector3(0, 0, 0), 0.0f };

            rasterizer::math::Vector3 sum(0, 0, 0);
            for (const auto& v : vertices)
                sum = sum + v;
            rasterizer::math::Vector3 center = sum * (1.0f / vertices.size());

            float maxDistSq = 0.0f;
            for (const auto& v : vertices)
            {
                rasterizer::math::Vector3 d = v - center;
                maxDistSq = std::max(maxDistSq, d.x * d.x + d.y * d.y + d.z * d.z);
            }

            return { center, std::sqrt(maxDistSq) };
        }
    };
}