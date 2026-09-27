#pragma once
#include "math/vector3.h"
#include "math/matrix4x4.h"

namespace rasterizer::scene {
    struct Camera {

        Camera() : position(0, 0, 0), orientation(math::Matrix4x4::identity()) {}
        Camera(const math::Vector3& position, const math::Matrix4x4& orientation)
            : position(position), orientation(orientation) {}

        math::Vector3 position;
        math::Matrix4x4 orientation;
    };
}