#pragma once

#include <vector>
#include "rendering/color.h"
#include "instance.h"
#include <SDL3/SDL.h>
#include "camera.h"
#include "light.h"

namespace rasterizer::scene
{
    struct Scene
    {
        Scene();
        ~Scene();

        Camera camera;

        rasterizer::rendering::Color backgroundColor;

        std::vector<Instance> instances;

        std::vector<Light> lights;
    };
}