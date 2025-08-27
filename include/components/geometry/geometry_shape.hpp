// -----------------------------------------------------------------------------
//
//  ray_tracer - triangle.hpp
//
// -----------------------------------------------------------------------------

#ifndef RAY_TRACER_GEOMETRY_SHAPE
#define RAY_TRACER_GEOMETRY_SHAPE

#include <vector>

#include "components/geometry/triangle.hpp"

struct triangle_shape
{
    triangle triangle;
};

struct mesh_shape
{
    std::vector<triangle> triangles;
};

struct sphere_shape
{
    vector3 center;
    float radius{};
};

#endif //RAY_TRACER_GEOMETRY_SHAPE
