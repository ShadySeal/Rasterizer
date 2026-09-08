#include "rasterizer.h"

using namespace rasterizer;

Rasterizer::Rasterizer(Canvas& canvas) : _canvas(canvas) {}

void Rasterizer::setBakcgroundColor(const int width, const int height, const ColorRGBA color) const
{
    for (int x = -width / 2; x <= width / 2; x++)
    {
        for (int y = -height / 2; y <= height / 2; y++)
        {
            _canvas.setPixel(x, y, color.packed);
        }
    }
}

void Rasterizer::drawLine(Vector2 p0, Vector2 p1, const ColorRGBA color) const
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
            _canvas.setPixel(
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
            _canvas.setPixel(
                static_cast<int>(std::round(xs[y - p0.y])),
                static_cast<int>(std::round(y)),
                color.packed);
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

void Rasterizer::drawWireframeTriangle(const Vector2 p0, const Vector2 p1, const Vector2 p2, const ColorRGBA color) const
{
    drawLine(p0, p1, color);
    drawLine(p1, p2, color);
    drawLine(p2, p0, color);
}

void Rasterizer::drawFilledTriangle(Vector2 p0, Vector2 p1, Vector2 p2, const ColorRGBA color) const
{
    // Sort the points so that y0 <= y1 <= y2
    if (p1.y < p0.y) { std::swap(p1, p0); }
    if (p2.y < p0.y) { std::swap(p2, p0); }
    if (p2.y < p1.y) { std::swap(p2, p1); }

    // Compute the x coordinates of the triangle edges
    auto x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
    auto x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
    auto x02 = interpolate(p0.y, p0.x, p2.y, p2.x);

    // Concatenate the short sides
    x01.pop_back();
    std::vector<float> x012 = x01;
    x012.insert(x012.end(), x12.begin(), x12.end());

    // Determine which is left and which is right
    std::vector<float> xLeft;
    std::vector<float> xRight;
    int m = std::floor(x012.size() / 2);
    if (x02[m] < x012[m])
    {
        xLeft = x02;
        xRight = x012;
    }
    else
    {
        xLeft = x012;
        xRight = x02;
    }

    // Draw the horizontal segments
    for (float y = p0.y; y <= p2.y; ++y)
    {
        for (float x = xLeft[y - p0.y]; x <= xRight[y - p0.y]; ++x)
        {
            _canvas.setPixel(x, y, color.packed);
        }
    }
}

void Rasterizer::drawShadedTriangle(Vector2 p0, Vector2 p1, Vector2 p2, const ColorRGBA color) const
{
    // Sort the points so that y0 <= y1 <= y2
    if (p1.y < p0.y) { std::swap(p1, p0); }
    if (p2.y < p0.y) { std::swap(p2, p0); }
    if (p2.y < p1.y) { std::swap(p2, p1); }

    // Define the heights at each vertex
    float h0 = 0.0f;
    float h1 = 0.0f;
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

            ColorRGBA shadedColor = color * h;

            _canvas.setPixel(
                static_cast<int>(std::round(x)),
                static_cast<int>(std::round(y)),
                shadedColor.packed
            );
        }
    }
}
