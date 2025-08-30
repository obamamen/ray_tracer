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
#include <cmath>
#include <algorithm>

// Minimum t for intersections to avoid self-hit
constexpr float MIN_T = 1e-4f;

// Small epsilon for front/back face test
constexpr float FACE_EPS = 1e-5f;

// ---------------------- Sphere Intersection ----------------------
inline bool intersect(const ray& r, const sphere& s, intersection& i)
{
    // oc = ray origin - sphere center
    const vector3 oc = r.origin - s.center;

    // Use half_b formulation for numeric stability
    const float a = vector3::dot(r.direction, r.direction);      // often 1 if normalized
    const float half_b = vector3::dot(oc, r.direction);
    const float c = vector3::dot(oc, oc) - s.radius * s.radius;

    const float disc = half_b * half_b - a * c;
    if (disc < 0.0f) return false;

    const float sqrt_disc = std::sqrt(disc);

    // Pick nearest positive root
    float t = (-half_b - sqrt_disc) / a;
    if (t < MIN_T) t = (-half_b + sqrt_disc) / a;
    if (t < MIN_T) return false;

    i.intersection_distance = t;

    // Outward normal
    vector3 hit_point = r.origin + r.direction * t;
    vector3 to_surface = hit_point - s.center;
    vector3 outward = to_surface.normalized();

    // Determine front/back face
    float cosr = vector3::dot(r.direction, outward);
    if (cosr > FACE_EPS) {
        i.back_face = true;
        i.normal = -outward;
    } else {
        i.back_face = false;
        i.normal = outward;
    }

    return true;
}

// ---------------------- Triangle Intersection ----------------------
inline bool intersect(const ray& r, const triangle& tri, intersection& i)
{
    vector3 edge1 = tri.v1 - tri.v0;
    vector3 edge2 = tri.v2 - tri.v0;

    vector3 h = vector3::cross(r.direction, edge2);
    float a = vector3::dot(edge1, h);

    if (std::fabs(a) < MIN_T) return false; // ray parallel

    float f = 1.0f / a;
    vector3 s = r.origin - tri.v0;
    float u = f * vector3::dot(s, h);
    if (u < 0.0f || u > 1.0f) return false;

    vector3 q = vector3::cross(s, edge1);
    float v = f * vector3::dot(r.direction, q);
    if (v < 0.0f || u + v > 1.0f) return false;

    float t = f * vector3::dot(edge2, q);
    if (t < MIN_T) return false;

    i.intersection_distance = t;

    // Compute outward normal
    vector3 outward = vector3::cross(edge1, edge2).normalized();

    // Front/back test
    float cosr = vector3::dot(r.direction, outward);
    if (cosr > FACE_EPS) {
        i.back_face = true;
        i.normal = -outward;
    } else {
        i.back_face = false;
        i.normal = outward;
    }

    return true;
}

#endif // RAY_TRACER_INTERSECTION_SYSTEM
