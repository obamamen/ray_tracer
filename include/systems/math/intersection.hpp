// -----------------------------------------------------------------------------
//
//  ray_tracer - intersection.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_INTERSECTION_SYSTEM
#define RAY_TRACER_INTERSECTION_SYSTEM
#include "components/geometry/sphere.hpp"
#include "components/geometry/triangle.hpp"
#include "components/math/ray.hpp"
#include "components/math/vector3.hpp"
#include "components/math/intersection.hpp"


inline bool intersect(const ray& r, const sphere& s, intersection& i)
{
    constexpr float EPSILON = 1e-6f;

    const vector3 oc = r.origin - s.center;
    const float a = vector3::dot(r.direction, r.direction);
    const float b = 2.0f * vector3::dot(oc, r.direction);
    const float c = vector3::dot(oc, oc) - s.radius * s.radius;

    const float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) return false;

    const float sqrt_disc = std::sqrt(discriminant);
    const float t1 = (-b - sqrt_disc) / (2.0f * a);
    const float t2 = (-b + sqrt_disc) / (2.0f * a);

    const float t = (t1 > EPSILON) ? t1 : ((t2 > EPSILON) ? t2 : -1.0f);
    if (t < EPSILON) return false;

    i.intersection_distance = t;
    i.normal = (r.origin + r.direction * t - s.center).normalized();

    return true;
}

inline bool intersect(const ray& r, const triangle& tri, intersection& i)
{
    constexpr float EPSILON = 1e-6f;

    const vector3 edge1 = tri.v1 - tri.v0;
    const vector3 edge2 = tri.v2 - tri.v0;

    const vector3 pv = vector3::cross(r.direction, edge2);
    const float det = vector3::dot(edge1, pv);

    if (det < EPSILON) return false;

    const float invDet = 1.0f / det;
    const vector3 tv = r.origin - tri.v0;

    const float uv = vector3::dot(tv, pv) * invDet;
    if (uv < 0.0f || uv > 1.0f) return false;

    const vector3 qv = vector3::cross(tv, edge1);
    if (const float v = vector3::dot(r.direction, qv) * invDet; v < 0.0f || uv + v > 1.0f) return false;

    const float t = vector3::dot(edge2, qv) * invDet;
    if (t < EPSILON) return false;

    i.intersection_distance = t;

    i.normal = vector3::cross(edge1, edge2).normalized();

    return true;
}

#endif//RAY_TRACER_INTERSECTION_SYSTEM