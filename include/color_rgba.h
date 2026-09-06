#pragma once

#include <SDL3/SDL.h>
#include <algorithm>

namespace rasterizer
{
    struct ColorRGBA
    {
        uint32_t packed;

        uint8_t red, green, blue, alpha;

        ColorRGBA(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
            : packed(SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha)),
            red(red), green(green), blue(blue), alpha(alpha) {}

        ColorRGBA operator+(const ColorRGBA& other) const
        {
            return ColorRGBA(
                std::min(255, red + other.red),
                std::min(255, green + other.green),
                std::min(255, blue + other.blue),
                alpha
            );
        }

        ColorRGBA operator*(const float value) const
        {
            return ColorRGBA(
                static_cast<uint8_t>(std::clamp(red * value, 0.0f, 255.0f)),
                static_cast<uint8_t>(std::clamp(green * value, 0.0f, 255.0f)),
                static_cast<uint8_t>(std::clamp(blue * value, 0.0f, 255.0f)),
                alpha
            );
        }
    };
}
