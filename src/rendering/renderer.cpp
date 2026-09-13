#include "rendering/renderer.h"
#include "rendering/rasterizer.h"
#include "scene/scene.h"
#include "rendering/color.h"
#include "math/vector3.h"

using namespace rasterizer::rendering;
using namespace rasterizer::math;

rasterizer::rendering::Renderer::Renderer(rasterizer::scene::Scene& scene, int wWidth, int wHeight, int wScale)
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
    rasterizer.setBakcgroundColor(_wWidth, _wHeight, Color(Color::WHITE));

    std::vector<Vector3> vertices = {
        { 1,  1,  1},
        {-1,  1,  1},
        {-1, -1,  1},
        { 1, -1,  1},
        { 1,  1, -1},
        {-1,  1, -1},
        {-1, -1, -1},
        { 1, -1, -1}
    };

    std::vector<Triangle> triangles = {
        {0, 1, 2, Color(Color::RED)},
        {0, 2, 3, Color(Color::RED)},
        {4, 0, 3, Color(Color::GREEN)},
        {4, 3, 7, Color(Color::GREEN)},
        {5, 4, 7, Color(Color::BLUE)},
        {5, 7, 6, Color(Color::BLUE)},
        {1, 5, 6, Color(Color::YELLOW)},
        {1, 6, 2, Color(Color::YELLOW)},
        {4, 5, 1, Color(Color::MAGENTA)},
        {4, 1, 0, Color(Color::MAGENTA)},
        {2, 6, 7, Color(Color::CYAN)},
        {2, 7, 3, Color(Color::CYAN)}
    };
    
    auto tranlatedVertices = rasterizer.translateVertices(Vector3(-1.5, 0, 7), vertices);
    rasterizer.renderObject(tranlatedVertices, triangles);

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
