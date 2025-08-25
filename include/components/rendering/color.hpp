// -----------------------------------------------------------------------------
//  
//  ray_tracer - color.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_COLOR
#define RAY_TRACER_COLOR

#include <algorithm>

struct color
{
    float r, g, b;

    color() : r(0), g(0), b(0) {}
    color(const float r, const float g, const float b) : r(r), g(g), b(b) {}

    explicit color(float x) : r(x), g(x), b(x) {}

    color operator+(const color& o) const { return {r + o.r, g + o.g, b + o.b}; }
    color operator*(const float t) const { return {r * t, g * t, b * t}; }
    color operator*(const color& o) const { return { r*o.r, g*o.g, b*o.b }; }
    color& operator+=(const color& o) { r+=o.r; g+=o.g; b+=o.b; return *this; }
    color& operator/=(const float x) { r /= x;g /= x;b /= x; return *this; }
    color operator/(const float x) const { return {r / x, g / x, b / x }; }

    [[nodiscard]] color clamped(const float min = 0.0f, const float max = 1.0f) const
    {
        return {
            std::min(std::max(r, min), max),
            std::min(std::max(g, min), max),
            std::min(std::max(b, min), max)
        };
    }

    [[nodiscard]] float luminance() const
    {
        return 0.2126f * r + 0.7152f * g + 0.0722f * b;
    }
};


#endif // RAY_TRACER_COLOR
