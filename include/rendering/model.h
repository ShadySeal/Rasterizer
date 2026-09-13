#pragma once

#include <string>
#include <vector>
#include "math/vector3.h"
#include "triangle.h"

using namespace rasterizer::math;

namespace rasterizer::rendering
{
    struct Model
    {
        std::string name;
        std::vector<Vector3> vertices;
        std::vector<Triangle> triangles;

        Model(std::string name, std::vector<Vector3> vertices, std::vector<Triangle> triangles)
            : name(name), vertices(vertices), triangles(triangles) {};
    };
}