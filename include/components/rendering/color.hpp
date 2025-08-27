// -----------------------------------------------------------------------------
//  
//  ray_tracer - color.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_COLOR
#define RAY_TRACER_COLOR

#include <algorithm>
#include <cstdint>

struct color
{
    float r, g, b;

    color() : r(0), g(0), b(0) {}
    color(const float r, const float g, const float b) : r(r), g(g), b(b) {}
    explicit color(const float x) : r(x), g(x), b(x) {}

    color operator+(const color& o) const { return {r + o.r, g + o.g, b + o.b}; }
    color operator*(const float t) const { return {r * t, g * t, b * t}; }
    color operator*(const color& o) const { return { r*o.r, g*o.g, b*o.b }; }
    color& operator*=(const float x) { r *= x; g *= x; b *= x; return *this; }
    color& operator+=(const color& o) { r+=o.r; g+=o.g; b+=o.b; return *this; }
    color& operator/=(const float x) { r /= x;g /= x;b /= x; return *this; }
    color operator/(const float x) const { return {r / x, g / x, b / x }; }

    static color from_bytes(const uint8_t r, const uint8_t g, const uint8_t b)
    {
        return {static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f};
    }

    static color from_hex(const uint32_t hex)
    {
        const uint8_t r = (hex >> 16) & 0xFF;
        const uint8_t g = (hex >> 8)  & 0xFF;
        const uint8_t b = hex & 0xFF;
        return from_bytes(r, g, b);
    }

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
