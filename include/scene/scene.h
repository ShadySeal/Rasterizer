#pragma once

#include <vector>
#include "rendering/color.h"
#include "rendering/instance.h"
#include "light.h"
#include <SDL3/SDL.h>

using namespace rasterizer::rendering;

namespace rasterizer::scene
{
    struct Scene
    {
        Scene();
        ~Scene();

        rasterizer::rendering::Color backgroundColor;

        std::vector<Instance> instances;

        std::vector<Light> lights;
    };
}