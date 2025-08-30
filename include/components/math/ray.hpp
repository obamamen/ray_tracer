// -----------------------------------------------------------------------------
//  
//  ray_tracer - ray.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_RAY
#define RAY_TRACER_RAY

#include "vector3.hpp"

struct ray
{
    vector3 origin;
    vector3 direction;


    ray() = default;
    ray(const vector3& o, const vector3& d) : origin(o), direction(d.normalized()) {}


    [[nodiscard]] vector3 at(const float t) const { return origin + direction * t; }
};

#endif // RAY_TRACER_RAY
