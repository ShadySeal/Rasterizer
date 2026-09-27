#pragma once

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/matrix4x4.h"
#include "color.h"
#include "canvas.h"
#include <vector>
#include "triangle.h"
#include "scene/instance.h"
#include "scene/scene.h"
#include "plane.h"

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

        rasterizer::math::Vector2 viewportToCanvas(float x, float y) const;
        std::vector<float> interpolate(const float i0, const float d0, const float i1, const float d1) const; 

        public:
        Rasterizer(Canvas& canvas, int cW, int cH, float vW, float vH, float d);

        rasterizer::math::Vector2 projectVertex(const rasterizer::math::Vector3& v) const; 
        void setBakcgroundColor(const int width, const int height, const Color color) const;

        // Drawing
        void drawLine(rasterizer::math::Vector2 p0, rasterizer::math::Vector2 p1, const Color color) const;
        void drawWireframeTriangle(const rasterizer::math::Vector2 p0, const rasterizer::math::Vector2 p1, const rasterizer::math::Vector2 p2, const Color color) const;
        void drawFilledTriangle(rasterizer::math::Vector2 p0, rasterizer::math::Vector2 p1, rasterizer::math::Vector2 p2, const Color color) const;
        void drawShadedTriangle(rasterizer::math::Vector2 p0, rasterizer::math::Vector2 p1, rasterizer::math::Vector2 p2, const Color color) const;

        // Rendering
        void renderScene(rasterizer::scene::Scene& scene) const;
        void renderTriangle(const Triangle& triangle, const std::vector<rasterizer::math::Vector2>& projected) const;
        void renderModel(const Model& model, const rasterizer::math::Matrix4x4& transform) const;

        rasterizer::scene::Scene clipScene(rasterizer::scene::Scene& scene, Plane& plane) const;
        rasterizer::scene::Instance clipInstance(rasterizer::scene::Instance& instance, Plane& plane) const;
        rasterizer::scene::Instance clipInstanceAgainstPlane(rasterizer::scene::Instance& instance, Plane& plane) const;
        std::vector<Triangle> clipTrianglesAgainstPlane(std::vector<Triangle>& triangles, Plane& plane) const;
        Triangle clipTriangle(Triangle& triangles, Plane& plane) const;

        float signedDistance(const Plane& plane, const rasterizer::math::Vector3& vertex) const;
    };
}