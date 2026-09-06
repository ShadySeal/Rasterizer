#pragma once

#include <cstdint>

namespace rasterizer
{
    class Canvas
    {
    private:
        uint32_t* _pixels;
        const int _width;
        const int _height;

    public:
        Canvas(int width, int height);
        ~Canvas();

        void setPixel(const int x, const int y, const uint32_t color) const;
        uint32_t* getPixels() const;
    };
}
