#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "color.h"
#include "canvas.h"
#include <vector>
#include "triangle.h"

using namespace rasterizer::math;

namespace rasterizer::rendering
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

        Vector2 projectVertex(const Vector3& v) const;
        void setBakcgroundColor(const int width, const int height, const Color color) const;
        void drawLine(Vector2 p0, Vector2 p1, const Color color) const;
        void drawWireframeTriangle(const Vector2 p0, const Vector2 p1, const Vector2 p2, const Color color) const;
        void drawFilledTriangle(Vector2 p0, Vector2 p1, Vector2 p2, const Color color) const;
        void drawShadedTriangle(Vector2 p0, Vector2 p1, Vector2 p2, const Color color) const;
        void renderObject(const std::vector<Vector3>& vertices, const std::vector<Triangle>& triangles) const;
        void renderTriangle(const Triangle& triangle, const std::vector<Vector2>& projected) const;
        std::vector<Vector3> translateVertices(const Vector3 t, const std::vector<Vector3>& vertices) const;
    };
}