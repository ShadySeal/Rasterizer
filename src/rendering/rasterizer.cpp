#include "rendering/rasterizer.h"

using namespace rasterizer::rendering;
using namespace rasterizer::math;
using namespace rasterizer::scene;

Rasterizer::Rasterizer(Canvas& canvas, int cW, int cH, float vW, float vH, float d)
: _canvas(canvas), _cW(cW), _cH(cH), _vW(vW), _vH(vH), _d(d)
{
    _depthBuffer.resize(_cW * _cH);
}

Vector2 Rasterizer::viewportToCanvas(float x, float y) const
{
    return Vector2(x * _cW / _vW, y * _cH / _vH);
}

Vector2 Rasterizer::projectVertex(const Vector3& v) const
{
    return viewportToCanvas(v.x * _d / v.z, v.y * _d / v.z);
}

void Rasterizer::setBakcgroundColor(const int width, const int height, const Color color) const
{
    for (int x = -width / 2; x <= width / 2; x++)
    {
        for (int y = -height / 2; y <= height / 2; y++)
        {
            _canvas.putPixel(x, y, color.packed);
        }
    }
}

std::vector<float> Rasterizer::interpolate(const float i0, const float d0, const float i1, const float d1) const
{
    if (i0 == i1)
    {
        return { d0 };
    }

    std::vector<float> values;
    float a = (d1 - d0) / (i1 - i0);
    float d = d0;

    for (float i = i0; i <= i1; ++i)
    {
        values.push_back(d);
        d = d + a;
    }

    return values;
}

void Rasterizer::drawLine(Vector2 p0, Vector2 p1, const Color color) const
{
    float dx = p0.x - p1.x;
    float dy = p0.y - p1.y;
    if (std::abs(p0.x - p1.x) > std::abs(p0.y - p1.y))
    {
        // Line is horizontal-ish
        // Make sure x0 < x1
        if (p0.x > p1.x)
        {
            std::swap(p0, p1);
        }

        auto ys = interpolate(p0.x, p0.y, p1.x, p1.y);

        for (float x = p0.x; x <= p1.x; ++x)
        {
            _canvas.putPixel(
                static_cast<int>(std::round(x)),
                static_cast<int>(std::round(ys[x - p0.x])),
                color.packed);
        }
    }
    else
    {
        // Line is vertical-ish
        // Make sure y0 < y1
        if (p0.y > p1.y)
        {
            std::swap(p0, p1);
        }

        auto xs = interpolate(p0.y, p0.x, p1.y, p1.x);

        for (float y = p0.y; y <= p1.y; ++y)
        {
            _canvas.putPixel(
                static_cast<int>(std::round(xs[y - p0.y])),
                static_cast<int>(std::round(y)),
                color.packed);
        }
    }
}

void Rasterizer::drawWireframeTriangle(const Vector2 p0, const Vector2 p1, const Vector2 p2, const Color color) const
{
    drawLine(p0, p1, color);
    drawLine(p1, p2, color);
    drawLine(p2, p0, color);
}

void Rasterizer::drawFilledTriangle(Vector3 p0, Vector3 p1, Vector3 p2, const Color color) const
{
    p0.y = std::round(p0.y);
    p1.y = std::round(p1.y);
    p2.y = std::round(p2.y);

    // Sort the points so that y0 <= y1 <= y2
    if (p1.y < p0.y) { std::swap(p1, p0); }
    if (p2.y < p0.y) { std::swap(p2, p0); }
    if (p2.y < p1.y) { std::swap(p2, p1); }

    float invZ0 = 1.0f / p0.z;
    float invZ1 = 1.0f / p1.z;
    float invZ2 = 1.0f / p2.z;

    // Compute the x coordinates of the triangle edges
    auto x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
    auto x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
    auto x02 = interpolate(p0.y, p0.x, p2.y, p2.x);

    // Compute the z values along the same edges, same way as x
    auto invZ01 = interpolate(p0.y, invZ0, p1.y, invZ1);
    auto invZ12 = interpolate(p1.y, invZ1, p2.y, invZ2);
    auto invZ02 = interpolate(p0.y, invZ0, p2.y, invZ2);

    // Concatenate the short sides
    x01.pop_back();
    std::vector<float> x012 = x01;
    x012.insert(x012.end(), x12.begin(), x12.end());

    invZ01.pop_back();
    std::vector<float> invZ012 = invZ01;
    invZ012.insert(invZ012.end(), invZ12.begin(), invZ12.end());


    // Determine which is left and which is right
    std::vector<float> xLeft, xRight, invZLeft, invZRight;
    int m = std::floor(x012.size() / 2);
    if (x02[m] < x012[m])
    {
        xLeft = x02;
        invZLeft = invZ02;

        xRight = x012;
        invZRight = invZ012;
    }
    else
    {
        xLeft = x012;
        invZLeft = invZ012;

        xRight = x02;
        invZRight = invZ02;
    }

    // Draw the horizontal segments
    for (float y = p0.y; y <= p2.y; ++y)
    {
        int row = static_cast<int>(y - p0.y);
        float xL = xLeft[row];
        float xR = xRight[row];

        auto zSegment = interpolate(xL, invZLeft[row], xR, invZRight[row]);

        for (float x = xL; x <= xR; ++x)
        {
            int col = static_cast<int>(x - xL);
            float z = zSegment[col];

            int ix = static_cast<int>(std::round(x));
            int iy = static_cast<int>(std::round(y));
            int idx = depthIndex(ix, iy);

            if (idx >= 0 && z > _depthBuffer[idx])
            {
                _canvas.putPixel(ix, iy, color.packed);
                _depthBuffer[idx] = z;
            }
        }
    }
}

void Rasterizer::drawShadedTriangle(Vector2 p0, Vector2 p1, Vector2 p2, const Color color) const
{
    p0.y = std::round(p0.y);
    p1.y = std::round(p1.y);
    p2.y = std::round(p2.y);

    // Sort the points so that y0 <= y1 <= y2
    if (p1.y < p0.y) { std::swap(p1, p0); }
    if (p2.y < p0.y) { std::swap(p2, p0); }
    if (p2.y < p1.y) { std::swap(p2, p1); }

    // Define the intensity at each vertex
    float h0 = 0.3f;
    float h1 = 0.1f;
    float h2 = 1.0f;

    // Compute the x coordinates and h values of the triangle edges
    auto x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
    auto h01 = interpolate(p0.y, h0, p1.y, h1);

    auto x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
    auto h12 = interpolate(p1.y, h1, p2.y, h2);

    auto x02 = interpolate(p0.y, p0.x, p2.y, p2.x);
    auto h02 = interpolate(p0.y, h0, p2.y, h2);

    // Concatenate the short sides
    x01.pop_back();
    std::vector<float> x012 = x01;
    x012.insert(x012.end(), x12.begin(), x12.end());

    h01.pop_back();
    std::vector<float> h012 = h01;
    h012.insert(h012.end(), h12.begin(), h12.end());

    // Determine which is left and which is right
    std::vector<float> xLeft;
    std::vector<float> hLeft;
    std::vector<float> xRight;
    std::vector<float> hRight;
    int m = std::floor(x012.size() / 2);
    if (x02[m] < x012[m])
    {
        xLeft = x02;
        hLeft = h02;

        xRight = x012;
        hRight = h012;
    }
    else
    {
        xLeft = x012;
        hLeft = h012;

        xRight = x02;
        hRight = h02;
    }

    // Draw the horizontal segments
    for (float y = p0.y; y <= p2.y; ++y)
    {
        auto xL = xLeft[y - p0.y];
        auto xR = xRight[y - p0.y];

        auto hSegment = interpolate(xL, hLeft[y - p0.y], xR, hRight[y - p0.y]);
        for (float x = xL; x <= xR; ++x)
        {
            int xIndex = static_cast<int>(x - xL);

            float h = std::clamp(hSegment[xIndex], 0.0f, 1.0f);

            Color shadedColor = color * h;

            _canvas.putPixel(
                static_cast<int>(std::round(x)),
                static_cast<int>(std::round(y)),
                shadedColor.packed
            );
        }
    }
}

void Rasterizer::renderScene(Scene& scene) const
{
    setBakcgroundColor(_cW, _cH, Color(Color::WHITE));
    clearDepthBuffer();

    auto frustumPlanes = Plane::makeFrustumPlanes();
    Scene clippedScene = clipScene(scene, frustumPlanes);

    for (auto& instance : clippedScene.instances)
    {
        renderModel(instance.model, scene);
    }
}

void Rasterizer::renderModel(const Model& model, Scene& scene) const
{
    std::vector<Vector3> projected;
    for (auto v : model.vertices)
    {
        projected.push_back(projectVertex(v).toVector3(v.z));
    }

    for (auto t : model.triangles)
    {
        Vector3 p0 = model.vertices[t.v[0]];
        Vector3 p1 = model.vertices[t.v[1]];
        Vector3 p2 = model.vertices[t.v[2]];

        // Triangle edges
        Vector3 edge1 = p1 - p0;
        Vector3 edge2 = p2 - p0;

        // Triangle normal
        Vector3 normal = Vector3::cross(edge1, edge2);

        // Camera is at (0, 0, 0) in camera space.
        Vector3 view(-p0.x, -p0.y, -p0.z);

        // Backface culling
        if (Vector3::dot(normal, view) <= 0)
        {
            continue;
        }

        // Lighting goes here, using the normal you already have
        Vector3 centroid = (p0 + p1 + p2) * (1.0f / 3.0f);
        float intensity = computeLighting(centroid, normal, scene);
        t.color = t.color * intensity;

        renderTriangle(t, projected);
    }
}

void Rasterizer::renderTriangle(const Triangle& triangle, const std::vector<Vector3>& projected) const
{
    drawFilledTriangle(projected[triangle.v[0]], projected[triangle.v[1]], projected[triangle.v[2]], triangle.color);
}

Scene Rasterizer::clipScene(Scene& scene, std::vector<Plane>& planes) const
{
    Matrix4x4 mCamera = Matrix4x4::makeCameraMatrix(scene.camera.position, scene.camera.orientation);

    std::vector<Instance> clippedInstances;
    for (auto& instance : scene.instances)
    {
        Instance cameraSpaceInstance = toCameraSpace(instance, mCamera);

        Instance clippedInstance = clipInstance(cameraSpaceInstance, planes);
        if (!clippedInstance.model.triangles.empty())
        {
            clippedInstances.push_back(clippedInstance);
        }
    }

    Scene clippedScene = scene;
    clippedScene.instances = clippedInstances;
    return clippedScene;
}

Instance Rasterizer::clipInstance(Instance& instance, std::vector<Plane>& planes) const
{
    for (auto& plane : planes)
    {
        auto clipped = clipInstanceAgainstPlane(instance, plane);
        if (!clipped.has_value())
        {
            instance.model.triangles.clear();
            break;
        }

        instance = *clipped;
        if (instance.model.triangles.empty())
        {
            break;
        }
    }

    return instance;
}

std::optional<Instance> Rasterizer::clipInstanceAgainstPlane(Instance& instance, Plane& plane) const
{
    float d = signedDistance(plane, instance.model.boundingSphere.center);
    float r = instance.model.boundingSphere.radius;
    if (d > r)
    {
        return instance;
    }
    else if (d < -r)
    {
        return std::nullopt;
    }   
    else
    {
        Instance clippedInstance = instance;
        clippedInstance.model.triangles = clipTrianglesAgainstPlane(clippedInstance.model.triangles, plane, clippedInstance.model.vertices);
        return clippedInstance;
    }
}

std::vector<Triangle> Rasterizer::clipTrianglesAgainstPlane(std::vector<Triangle>& triangles, Plane& plane, std::vector<Vector3>& vertices) const
{
    std::vector<Triangle> clippedTriangles;

    for (auto& triangle : triangles)
    {
        auto clipped = clipTriangle(triangle, plane, vertices);

        for (auto& t : clipped)
        {
            clippedTriangles.push_back(t);
        }
    }

    return clippedTriangles;
}

std::vector<Triangle> Rasterizer::clipTriangle(const Triangle& triangle, const Plane& plane, std::vector<Vector3>& vertices) const
{
    Vector3 v0 = vertices[triangle.v[0]];
    Vector3 v1 = vertices[triangle.v[1]];
    Vector3 v2 = vertices[triangle.v[2]];

    float d0 = signedDistance(plane, v0);
    float d1 = signedDistance(plane, v1);
    float d2 = signedDistance(plane, v2);

    // All vertices are in front of the plane.
    if (d0 > 0 && d1 > 0 && d2 > 0)
    {
        return { triangle };
    }

    // All vertices are behind the plane.
    if (d0 < 0 && d1 < 0 && d2 < 0)
    {
        return {};
    }

    // Only v0 is in front.
    if (d0 > 0 && d1 < 0 && d2 < 0)
    {
        Vector3 B = intersect(v0, v1, plane);
        Vector3 C = intersect(v0, v2, plane);

        vertices.push_back(B);
        vertices.push_back(C);

        Triangle clippedTriangle = Triangle(0, 0, 0, triangle.color);
        clippedTriangle.v[0] = triangle.v[0];
        clippedTriangle.v[1] = vertices.size() - 2;
        clippedTriangle.v[2] = vertices.size() - 1;
        clippedTriangle.color = triangle.color;

        return { clippedTriangle };
    }

    // Only v1 is in front.
    if (d1 > 0 && d0 < 0 && d2 < 0)
    {
        Vector3 A = intersect(v1, v0, plane);
        Vector3 C = intersect(v1, v2, plane);

        vertices.push_back(A);
        vertices.push_back(C);

        Triangle clippedTriangle = Triangle(0, 0, 0, triangle.color);
        clippedTriangle.v[0] = triangle.v[1];
        clippedTriangle.v[1] = vertices.size() - 2;
        clippedTriangle.v[2] = vertices.size() - 1;
        clippedTriangle.color = triangle.color;

        return { clippedTriangle };
    }

    // Only v2 is in front.
    if (d2 > 0 && d0 < 0 && d1 < 0)
    {
        Vector3 A = intersect(v2, v0, plane);
        Vector3 B = intersect(v2, v1, plane);

        vertices.push_back(A);
        vertices.push_back(B);

        Triangle clippedTriangle = Triangle(0, 0, 0, triangle.color);
        clippedTriangle.v[0] = triangle.v[2];
        clippedTriangle.v[1] = vertices.size() - 2;
        clippedTriangle.v[2] = vertices.size() - 1;
        clippedTriangle.color = triangle.color;

        return { clippedTriangle };
    }

    // Only v0 is behind.
    if (d0 < 0 && d1 > 0 && d2 > 0)
    {
        Vector3 A = intersect(v0, v1, plane);
        Vector3 B = intersect(v0, v2, plane);

        vertices.push_back(A);
        vertices.push_back(B);

        int a = vertices.size() - 2;
        int b = vertices.size() - 1;

        Triangle t1 = Triangle(0, 0, 0, triangle.color);
        t1.v[0] = triangle.v[1];
        t1.v[1] = triangle.v[2];
        t1.v[2] = a;
        t1.color = triangle.color;

        Triangle t2 = Triangle(0, 0, 0, triangle.color);
        t2.v[0] = a;
        t2.v[1] = triangle.v[2];
        t2.v[2] = b;
        t2.color = triangle.color;

        return { t1, t2 };
    }

    // Only v1 is behind.
    if (d1 < 0 && d0 > 0 && d2 > 0)
    {
        Vector3 A = intersect(v1, v0, plane);
        Vector3 B = intersect(v1, v2, plane);

        vertices.push_back(A);
        vertices.push_back(B);

        int a = vertices.size() - 2;
        int b = vertices.size() - 1;

        Triangle t1 = Triangle(0, 0, 0, triangle.color);
        t1.v[0] = triangle.v[0];
        t1.v[1] = triangle.v[2];
        t1.v[2] = a;
        t1.color = triangle.color;

        Triangle t2 = Triangle(0, 0, 0, triangle.color);
        t2.v[0] = a;
        t2.v[1] = triangle.v[2];
        t2.v[2] = b;
        t2.color = triangle.color;

        return { t1, t2 };
    }

    // Only v2 is behind.
    if (d2 < 0 && d0 > 0 && d1 > 0)
    {
        Vector3 A = intersect(v2, v0, plane);
        Vector3 B = intersect(v2, v1, plane);

        vertices.push_back(A);
        vertices.push_back(B);

        int a = vertices.size() - 2;
        int b = vertices.size() - 1;

        Triangle t1 = Triangle(0, 0, 0, triangle.color);
        t1.v[0] = triangle.v[0];
        t1.v[1] = triangle.v[1];
        t1.v[2] = a;
        t1.color = triangle.color;

        Triangle t2 = Triangle(0, 0, 0, triangle.color);
        t2.v[0] = a;
        t2.v[1] = triangle.v[1];
        t2.v[2] = b;
        t2.color = triangle.color;

        return { t1, t2 };
    }

    return {};
}

Instance Rasterizer::toCameraSpace(const Instance& instance, const Matrix4x4& mCamera) const
{
    Matrix4x4 m = mCamera * Matrix4x4::fromTransform(instance.transform);

    Instance result = instance;
    for (auto& v : result.model.vertices)
    {
        v = m * v;
    }
    result.model.boundingSphere.center = m * instance.model.boundingSphere.center;
    result.model.boundingSphere.radius = instance.model.boundingSphere.radius * instance.transform.scale;
    result.transform = Transform{};

    return result;
}

float Rasterizer::signedDistance(const Plane& plane, const rasterizer::math::Vector3& vertex) const
{
    return Vector3::dot(plane.normal, vertex) + plane.d;
}

Vector3 Rasterizer::intersect(const Vector3& A, const Vector3& B, const Plane& plane) const
{
    float dA = signedDistance(plane, A);
    float dB = signedDistance(plane, B);
    float t = dA / (dA - dB);
    return A + (B - A) * t;
}

void Rasterizer::clearDepthBuffer() const
{
    std::fill(_depthBuffer.begin(), _depthBuffer.end(), 0.0f);;
}

int Rasterizer::depthIndex(int x, int y) const
{
    int ix = x + _cW / 2;
    int iy = y + _cH / 2;

    if (ix < 0 || ix >= _cW || iy < 0 || iy >= _cH)
        return -1; // out of bounds

    return iy * _cW + ix;
}

float Rasterizer::computeLighting(const Vector3 p, const Vector3 n, Scene& scene) const
{
    float i = 0.0f;

    for (auto& light : scene.lights)
    {
        if (light.type == Light::AMBIENT)
        {
            i += light.getIntensity();
        }
        else
        {
            Vector3 l;

            if (light.type == Light::POINT)
            {
                l = light.position - p;
            }
            else
            {
                l = light.direction;
            }

            float nDotL = Vector3::dot(n, l);

            if (nDotL > 0)
            {
                i += light.getIntensity() * nDotL/(n.magnitude() * l.magnitude());
            }
        }
    }

    return i;
}
