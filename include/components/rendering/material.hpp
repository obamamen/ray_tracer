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
    color emission;


    material() : albedo(1,1,1), reflectivity(0), emission(0,0,0) {}
    material(const color& c, float r, const color& e=color(0,0,0)) : albedo(c), reflectivity(r), emission(e) {}
};

#endif // RAY_TRACER_MATERIAL
