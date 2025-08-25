// -----------------------------------------------------------------------------
//  
//  ray_tracer - triangle.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_TRIANGLE
#define RAY_TRACER_TRIANGLE

#include "components/math/vector3.hpp"

struct triangle
{
    vector3 v0, v1, v2;

    triangle() = default;
    triangle(const vector3& a, const vector3& b, const vector3& c) : v0(a), v1(b), v2(c) {}

    [[nodiscard]] vector3 normal() const
    {
        return vector3::cross(v1-v0, v2-v0).normalized();
    }

    [[nodiscard]] vector3 centroid() const
    {
        return (v0 + v1 + v2) / 3.0f;
    }

    [[nodiscard]] float area() const
    {
        return 0.5f * vector3::cross(v1-v0, v2-v0).length();
    }

    [[nodiscard]] bool contains_point(const vector3& p) const
    {
        const vector3 n = normal();
        const vector3 c0 = vector3::cross(v1-v0, p-v0);
        const vector3 c1 = vector3::cross(v2-v1, p-v1);
        const vector3 c2 = vector3::cross(v0-v2, p-v2);
        return vector3::dot(n, c0)>=0 && vector3::dot(n, c1)>=0 && vector3::dot(n, c2)>=0;
    }
};

#endif // RAY_TRACER_TRIANGLE
