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
    color albedo{0,0,0};
    float reflectivity{0};
    color emission{0,0,0};

    material() : albedo(1.0f, 0.75f, 0.8f) {}
    material(const color& c, const float r, const color& e=color(0,0,0)) : albedo(c), reflectivity(r), emission(e) {}
};

#endif // RAY_TRACER_MATERIAL
