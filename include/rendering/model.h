#pragma once

#include <string>
#include <vector>
#include "math/vector3.h"
#include "triangle.h"
#include "boundingSphere.h"

namespace rasterizer::rendering
{
    struct Model
    {
        std::string name;
        std::vector<rasterizer::math::Vector3> vertices;
        std::vector<Triangle> triangles;
        BoundingSphere boundingSphere;

        Model(std::string name, std::vector<rasterizer::math::Vector3> vertices, std::vector<Triangle> triangles)
            : name(name), vertices(vertices), triangles(triangles) 
        {
            boundingSphere = computeBoundingSphere(vertices);
        };

        BoundingSphere computeBoundingSphere(const std::vector<rasterizer::math::Vector3>& vertices)
        {
            if (vertices.empty()) return { rasterizer::math::Vector3(0, 0, 0), 0.0f };

            rasterizer::math::Vector3 sum(0, 0, 0);
            for (const auto& v : vertices)
                sum = sum + v;
            rasterizer::math::Vector3 center = sum * (1.0f / vertices.size());

            float maxDistSq = 0.0f;
            for (const auto& v : vertices)
            {
                rasterizer::math::Vector3 d = v - center;
                maxDistSq = std::max(maxDistSq, d.x * d.x + d.y * d.y + d.z * d.z);
            }

            return { center, std::sqrt(maxDistSq) };
        }

        static Model makeSphere(const std::string& name, float radius, int latitudeSegments, int longitudeSegments, const Color& color)
        {
            std::vector<rasterizer::math::Vector3> vertices;
            std::vector<Triangle> triangles;

            // Generate vertices
            for (int lat = 0; lat <= latitudeSegments; ++lat)
            {
                float theta =
                    M_PI * static_cast<float>(lat) / latitudeSegments;

                float sinTheta = std::sin(theta);
                float cosTheta = std::cos(theta);

                for (int lon = 0; lon < longitudeSegments; ++lon)
                {
                    float phi =
                        2.0f * M_PI * static_cast<float>(lon) / longitudeSegments;

                    float sinPhi = std::sin(phi);
                    float cosPhi = std::cos(phi);

                    float x = radius * sinTheta * cosPhi;
                    float y = radius * cosTheta;
                    float z = radius * sinTheta * sinPhi;

                    vertices.push_back(rasterizer::math::Vector3(x, y, z));
                }
            }

            // Generate triangles
            for (int lat = 0; lat < latitudeSegments; ++lat)
            {
                for (int lon = 0; lon < longitudeSegments; ++lon)
                {
                    int current =
                        lat * longitudeSegments + lon;

                    int next =
                        lat * longitudeSegments +
                        (lon + 1) % longitudeSegments;

                    int below =
                        (lat + 1) * longitudeSegments + lon;

                    int belowNext =
                        (lat + 1) * longitudeSegments +
                        (lon + 1) % longitudeSegments;

                    triangles.push_back(
                        Triangle(current, below, next, color)
                    );

                    triangles.push_back(
                        Triangle(next, below, belowNext, color)
                    );
                }
            }

            return Model(name, vertices, triangles);
        }
    };
}