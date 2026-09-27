#pragma once

#include "rendering/model.h"
#include "math/vector3.h"
#include "math/transform.h"

namespace rasterizer::scene
{
    struct Instance
    {
        rasterizer::rendering::Model model;
        rasterizer::math::Vector3 position;
        rasterizer::math::Transform transform;

        Instance(rasterizer::rendering::Model model, rasterizer::math::Vector3 position, rasterizer::math::Transform transform)
            : model(model), position(position), transform(transform) {};
    };
    
}