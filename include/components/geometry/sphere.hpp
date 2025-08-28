// -----------------------------------------------------------------------------
//
//  ray_tracer - sphere.hpp
//
// -----------------------------------------------------------------------------


#ifndef RAY_TRACER_SPHERE
#define RAY_TRACER_SPHERE
#include "components/math/vector3.hpp"

struct sphere
{
    vector3 center;
    float radius{};

    sphere(const vector3 center, const float radius) : center(center), radius(radius) {}
};

#endif //RAY_TRACER_SPHERE
