#pragma once

#include <cmath>

namespace rasterizer
{
    struct Vector2
    {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}

        Vector2 operator+(const Vector2& other) const
        {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 operator-(const Vector2& other) const
        {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2 operator*(float scalar) const
        {
            return Vector2(x * scalar, y * scalar);
        }

        Vector2 operator/(float scalar) const
        {
            return Vector2(x / scalar, y / scalar);
        }

        float magnitude() const
        {
            return std::sqrt(dot(*this, *this));
        }

        void normalize()
        {
            float mag = magnitude();
            x /= mag;
            y /= mag;
        }

        static float dot(const Vector2& a, const Vector2& b)
        {
            return a.x * b.x + a.y * b.y;
        }
    };
}
