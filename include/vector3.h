#pragma once

#include <cmath>

namespace rasterizer
{
    struct Vector3
    {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

        Vector3 operator+(const Vector3& other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator-(const Vector3& other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator*(float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }

        Vector3 operator/(float scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
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
            z /= mag;
        }

        static float dot(const Vector3& a, const Vector3& b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }
    };
}
