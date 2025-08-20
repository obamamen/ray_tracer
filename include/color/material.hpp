// -----------------------------------------------------------------------------
//  
//  ray_tracer - material.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_MATERIAL
#define RAY_TRACER_MATERIAL

#include "color.hpp"

struct material
{
    color albedo;
    float reflectivity;


    material() : albedo(1,1,1), reflectivity(0) {}
    material(const color& c, const float reflectivity) : albedo(c), reflectivity(reflectivity) {}
};

#endif // RAY_TRACER_MATERIAL
