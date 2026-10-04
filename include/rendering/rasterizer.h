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

        mutable std::vector<float> _depthBuffer;

        rasterizer::math::Vector2 viewportToCanvas(float x, float y) const;
        std::vector<float> interpolate(const float i0, const float d0, const float i1, const float d1) const; 
        rasterizer::math::Vector2 projectVertex(const rasterizer::math::Vector3& v) const; 
        void setBakcgroundColor(const int width, const int height, const Color color) const;

        // Drawing
        void drawLine(rasterizer::math::Vector2 p0, rasterizer::math::Vector2 p1, const Color color) const;
        void drawWireframeTriangle(const rasterizer::math::Vector2 p0, const rasterizer::math::Vector2 p1, const rasterizer::math::Vector2 p2, const Color color) const;
        void drawFilledTriangle(rasterizer::math::Vector3 p0, rasterizer::math::Vector3 p1, rasterizer::math::Vector3 p2, const Color color) const;
        void drawShadedTriangle(rasterizer::math::Vector2 p0, rasterizer::math::Vector2 p1, rasterizer::math::Vector2 p2, const Color color) const;

        // Rendering
        void renderTriangle(const Triangle& triangle, const std::vector<rasterizer::math::Vector3>& projected) const;
        void renderModel(const Model& model) const;

        rasterizer::scene::Scene clipScene(rasterizer::scene::Scene& scene, std::vector<Plane>& planes) const;
        rasterizer::scene::Instance clipInstance(rasterizer::scene::Instance& instance, std::vector<Plane>& planes) const;
        std::optional<rasterizer::scene::Instance> clipInstanceAgainstPlane(rasterizer::scene::Instance& instance, Plane& planes) const;
        std::vector<Triangle> clipTrianglesAgainstPlane(std::vector<Triangle>& triangles, Plane& plane, std::vector<rasterizer::math::Vector3>& vertices) const;
        std::vector<Triangle> clipTriangle(const Triangle& triangle, const Plane& plane, std::vector<rasterizer::math::Vector3>& vertices) const;

        rasterizer::scene::Instance toCameraSpace(const rasterizer::scene::Instance& instance, const rasterizer::math::Matrix4x4& mCamera) const;
        float signedDistance(const Plane& plane, const rasterizer::math::Vector3& vertex) const;
        rasterizer::math::Vector3 intersect(const rasterizer::math::Vector3& A, const rasterizer::math::Vector3& B, const Plane& plane) const;

        void clearDepthBuffer() const;
        int depthIndex(int x, int y) const;

        public:
        Rasterizer(Canvas& canvas, int cW, int cH, float vW, float vH, float d);

        void renderScene(rasterizer::scene::Scene& scene) const;
    };
}