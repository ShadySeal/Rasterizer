#include "renderer.h"
#include "scene.h"
#include "rasterizer.h"
#include "color_rgba.h"
#include "vector3.h"

using namespace rasterizer;

Renderer::Renderer(Scene& scene, int wWidth, int wHeight, int wScale)
    : _scene(scene), _wWidth(wWidth), _wHeight(wHeight), _wScale(wScale)
{
    _initialized = init();
}

Renderer::~Renderer()
{
    cleanup();
}

bool Renderer::logSdlError(const char* message)
{
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s: %s\n", message, SDL_GetError());
    return false;
}

bool Renderer::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return logSdlError("Could not initialize SDL");
    }
    
    if (!SDL_CreateWindowAndRenderer("Renderer Window", _wWidth * _wScale, _wHeight * _wScale, 0, &_window, &_renderer))
    {
        return logSdlError("Could not create window and renderer");
    }

    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, _wWidth, _wHeight);
    if (!_texture)
    {
        return logSdlError("Could not create texture");
    }

    if (_wScale > 1 && !SDL_SetTextureScaleMode(_texture, SDL_SCALEMODE_NEAREST))
    {
        return logSdlError("Could not set texture scale mode");
    }

    _canvas = std::make_unique<Canvas>(_wWidth, _wHeight);

    return true;
}

void Renderer::render()
{
    if (!_initialized) return;

    bool running = true;

    Rasterizer rasterizer(*_canvas, _wWidth, _wHeight, 1, 1, 1);
    rasterizer.setBakcgroundColor(_wWidth, _wHeight, ColorRGBA(255, 255, 255));

    // The four "front" vertices'
    Vector3 vAf(-2, -0.5, 5);
    Vector3 vBf(-2, 0.5, 5);
    Vector3 vCf(-1, 0.5, 5);
    Vector3 vDf(-1, -0.5, 5);

    // The four "back" vertices
    Vector3 vAb(-2, -0.5, 6);
    Vector3 vBb(-2, 0.5, 6);
    Vector3 vCb(-1, 0.5, 6);
    Vector3 vDb(-1, -0.5, 6);

    // The front face
    rasterizer.drawLine(rasterizer.projectVertex(vAf), rasterizer.projectVertex(vBf), ColorRGBA(0, 0, 255));
    rasterizer.drawLine(rasterizer.projectVertex(vBf), rasterizer.projectVertex(vCf), ColorRGBA(0, 0, 255));
    rasterizer.drawLine(rasterizer.projectVertex(vCf), rasterizer.projectVertex(vDf), ColorRGBA(0, 0, 255));
    rasterizer.drawLine(rasterizer.projectVertex(vDf), rasterizer.projectVertex(vAf), ColorRGBA(0, 0, 255));

    // The back face
    rasterizer.drawLine(rasterizer.projectVertex(vAb), rasterizer.projectVertex(vBb), ColorRGBA(255, 0, 0));
    rasterizer.drawLine(rasterizer.projectVertex(vBb), rasterizer.projectVertex(vCb), ColorRGBA(255, 0, 0));
    rasterizer.drawLine(rasterizer.projectVertex(vCb), rasterizer.projectVertex(vDb), ColorRGBA(255, 0, 0));
    rasterizer.drawLine(rasterizer.projectVertex(vDb), rasterizer.projectVertex(vAb), ColorRGBA(255, 0, 0));

    // The front-to-back edges
    rasterizer.drawLine(rasterizer.projectVertex(vAf), rasterizer.projectVertex(vAb), ColorRGBA(0, 255, 0));
    rasterizer.drawLine(rasterizer.projectVertex(vBf), rasterizer.projectVertex(vBb), ColorRGBA(0, 255, 0));
    rasterizer.drawLine(rasterizer.projectVertex(vCf), rasterizer.projectVertex(vCb), ColorRGBA(0, 255, 0));
    rasterizer.drawLine(rasterizer.projectVertex(vDf), rasterizer.projectVertex(vDb), ColorRGBA(0, 255, 0));

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_UpdateTexture(_texture, nullptr, _canvas->getPixels(), _wWidth * sizeof(uint32_t));
        SDL_RenderClear(_renderer);
        SDL_RenderTexture(_renderer, _texture, nullptr, nullptr);
        SDL_RenderPresent(_renderer);
    }
}

void Renderer::cleanup()
{
    if (_texture)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }

    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }

    if (_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }

    _canvas.reset();
    
    SDL_Quit();
}
