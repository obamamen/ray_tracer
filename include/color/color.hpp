// -----------------------------------------------------------------------------
//  
//  ray_tracer - color.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_COLOR
#define RAY_TRACER_COLOR

struct color
{
    float r, g, b;

    color() : r(0), g(0), b(0) {}
    color(const float rr, const float gg, const float bb) : r(rr), g(gg), b(bb) {}

    color operator+(const color& o) const { return {r + o.r, g + o.g, b + o.b}; }
    color operator*(const float t) const { return {r * t, g * t, b * t}; }
    color& operator+=(const color& o) { r+=o.r; g+=o.g; b+=o.b; return *this; }
};


#endif // RAY_TRACER_COLOR
