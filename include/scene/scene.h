#pragma once

#include <vector>
#include "rendering/color.h"
#include "light.h"
#include <SDL3/SDL.h>

namespace rasterizer::scene
{
    struct Scene
    {
        Scene();
        ~Scene();

        rasterizer::rendering::Color backgroundColor;

        std::vector<Light> lights;
    };
}