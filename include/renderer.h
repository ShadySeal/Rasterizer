#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <memory>
#include <string>
#include "canvas.h"

namespace rasterizer
{
    struct Scene;
    
    class Renderer
    {
    private:
        Scene& _scene;
        SDL_Window* _window = nullptr;
        SDL_Renderer* _renderer = nullptr;
        SDL_Texture* _texture = nullptr;
        std::unique_ptr<Canvas> _canvas;

        const int _wWidth;
        const int _wHeight;
        const int _wScale;
        bool _initialized = false;

        bool init();
        bool logSdlError(const char* message);
        void cleanup();

    public:
        Renderer(Scene& scene, int cW, int cH, int wS = 1);
        ~Renderer();

        void render();
    };
}
