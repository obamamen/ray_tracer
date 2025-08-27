// -----------------------------------------------------------------------------
//
//  ray_tracer - scene.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_SCENE
#define RAY_TRACER_SCENE
#include <vector>

#include "components/scene/object.hpp"

struct environment
{
    color fog_color = color(0, 0, 0);
    float fog_density = 0.01;
    color sky_color = color(0.1, 0.1, 0.2);
};

struct scene
{
    private:
    std::vector<object> objects;
    std::vector<material> materials;
};

#endif //RAY_TRACER_SCENE
