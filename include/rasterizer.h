#pragma once

#include "vector2.h"
#include "vector3.h"
#include "color_rgba.h"
#include "canvas.h"
#include <vector>

namespace rasterizer
{
    class Rasterizer
    {
        private:
        Canvas& _canvas;
        const int _cW;
        const int _cH;
        const float _vW;
        const float _vH;
        const float _d;

        std::vector<float> interpolate(const float i0, const float d0, const float i1, const float d1) const;
        Vector2 viewportToCanvas(float x, float y) const;

        public:
        Rasterizer(Canvas& canvas, int cW, int cH, float vW, float vH, float d);

        void setBakcgroundColor(const int width, const int height, const ColorRGBA color) const;
        void drawLine(Vector2 p0, Vector2 p1, const ColorRGBA color) const;
        void drawWireframeTriangle(const Vector2 p0, const Vector2 p1, const Vector2 p2, const ColorRGBA color) const;
        void drawFilledTriangle(Vector2 p0, Vector2 p1, Vector2 p2, const ColorRGBA color) const;
        void drawShadedTriangle(Vector2 p0, Vector2 p1, Vector2 p2, const ColorRGBA color) const;
        Vector2 projectVertex(const Vector3& v) const;
    };
}