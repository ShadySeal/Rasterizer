#pragma once

#include <vector>
#include "color_rgba.h"
#include "light.h"
#include <SDL3/SDL.h>

namespace rasterizer
{
    struct Scene
    {
        Scene();
        ~Scene();

        ColorRGBA backgroundColor;

        std::vector<Light> lights;
    };
}