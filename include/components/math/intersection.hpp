// -----------------------------------------------------------------------------
//
//  ray_tracer - intersection.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_INTERSECTION_COMPONENT
#define RAY_TRACER_INTERSECTION_COMPONENT
#include <cfloat>
#include "vector3.hpp"

struct intersection
{
    float intersection_distance{FLT_MAX};
    vector3 normal;
    bool back_face{false};
};

#endif//RAY_TRACER_INTERSECTION_COMPONENT