#pragma once

#include <SDL3/SDL.h>
#include <algorithm>
#include <optional>

namespace rasterizer::rendering
{
    struct Color
    {
        enum Name
        {
            BLACK,
            WHITE,
            RED,
            GREEN,
            BLUE,
            YELLOW,
            CYAN,
            MAGENTA
        };

        uint32_t packed;
        uint8_t red, green, blue, alpha;

        // RGB constructor
        Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255)
            : packed(SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha)),
            red(red), green(green), blue(blue), alpha(alpha) {}

        // Named color constructor
        Color(Name name) : Color()
        {
            switch (name)
            {
                case Name::BLACK:
                    red = 0;
                    green = 0;
                    blue = 0;
                    break;

                case Name::WHITE:
                    red = 255;
                    green = 255;
                    blue = 255;
                    break;

                case Name::RED:
                    red = 255;
                    green = 0;
                    blue = 0;
                    break;

                case Name::GREEN:
                    red = 0;
                    green = 255;
                    blue = 0;
                    break;

                case Name::BLUE:
                    red = 0;
                    green = 0;
                    blue = 255;
                    break;

                case Name::YELLOW:
                    red = 255;
                    green = 255;
                    blue = 0;
                    break;

                case Name::CYAN:
                    red = 0;
                    green = 255;
                    blue = 255;
                    break;

                case Name::MAGENTA:
                    red = 255;
                    green = 0;
                    blue = 255;
                    break;

                default:
                    break;
            }

            packed = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, red, green, blue, alpha);
        }

        Color operator+(const Color& other) const
        {
            return Color(
                std::min(255, red + other.red),
                std::min(255, green + other.green),
                std::min(255, blue + other.blue),
                alpha
            );
        }

        Color operator*(const float value) const
        {
            return Color(
                static_cast<uint8_t>(std::clamp(red * value, 0.0f, 255.0f)),
                static_cast<uint8_t>(std::clamp(green * value, 0.0f, 255.0f)),
                static_cast<uint8_t>(std::clamp(blue * value, 0.0f, 255.0f)),
                alpha
            );
        }
    };
}
