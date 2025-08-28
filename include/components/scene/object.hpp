// -----------------------------------------------------------------------------
//
//  ray_tracer - object.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_OBJECT
#define RAY_TRACER_OBJECT
#include <variant>

#include "components/geometry/triangle.hpp"
#include "components/geometry/sphere.hpp"
#include "components/rendering/material.hpp"

struct object
{
    std::variant<triangle,sphere> shape;
    material mat;

    template <typename Shape>
    explicit object(Shape&& s, const material m)
        : shape(std::forward<Shape>(s)), mat(m) {}
};

#endif //RAY_TRACER_OBJECT
