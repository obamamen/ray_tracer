// -----------------------------------------------------------------------------
//
//  ray_tracer - object.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_OBJECT
#define RAY_TRACER_OBJECT
#include <variant>

#include "components/geometry/geometry_shape.hpp"
#include "components/rendering/material.hpp"

struct object
{
    std::variant<triangle_shape,mesh_shape,sphere_shape> shape;
    material *material = nullptr;
};

#endif //RAY_TRACER_OBJECT
