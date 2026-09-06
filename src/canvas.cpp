#include "canvas.h"

using namespace rasterizer;

Canvas::Canvas(int width, int height) : _width(width), _height(height)
{
    _pixels = new uint32_t[width * height];
}

Canvas::~Canvas()
{
    delete[] _pixels;
}

void Canvas::setPixel(const int x, const int y, const uint32_t color) const
{
    const int sx = _width / 2 + x;
    const int sy = _height / 2 - y;
    if (sx >= 0 && sx < _width && sy >= 0 && sy < _height)
    {
        _pixels[sy * _width + sx] = color;
    }
}

uint32_t* Canvas::getPixels() const
{
    return _pixels;
}
