#pragma once

#include "model.h"
#include "math/vector3.h"

using namespace rasterizer::math;

namespace rasterizer::rendering
{
    struct Instance
    {
        Model model;
        Vector3 position;

        Instance(Model model, Vector3 position) : model(model), position(position) {};
    };
    
}