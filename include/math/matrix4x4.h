#pragma once

#include "vector3.h"
#include "transform.h"

namespace rasterizer::math
{
    struct Matrix4x4
    {
        float m[4][4];

        static Matrix4x4 identity()
        {
            return {{
                {1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}
            }};
        }

        // scale * rotate * translate combined
        static Matrix4x4 fromTransform(const Transform& t)
        {
            float cosTheta = std::cos(t.rotation);
            float sinTheta = std::sin(t.rotation);

            return {{
                {t.scale * cosTheta,  0, t.scale * sinTheta, t.translation.x},
                {0,                   t.scale, 0,             t.translation.y},
                {-t.scale * sinTheta, 0, t.scale * cosTheta, t.translation.z},
                {0,                   0, 0,                   1}
            }};
        }

        static Matrix4x4 makeCameraMatrix(Vector3 position, Matrix4x4 orientation)
        {
            Matrix4x4 translation = {{
                {1, 0, 0, -position.x},
                {0, 1, 0, -position.y},
                {0, 0, 1, -position.z},
                {0, 0, 0, 1}
            }};

            return orientation * translation;
        }

        // composition
        Matrix4x4 operator*(const Matrix4x4& other) const
        {
            Matrix4x4 result;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    result.m[i][j] = 0;
                    for (int k = 0; k < 4; ++k)
                    {
                        result.m[i][j] += m[i][k] * other.m[k][j];
                    }
                }
            }
            return result;
        }

        // apply to a point
        Vector3 operator*(const Vector3& v) const
        {
            float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
            float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
            float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
            float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];

            if (w != 0.0f)
            {
                x /= w;
                y /= w;
                z /= w;
            }

            return Vector3(x, y, z);
        } 
    };
}