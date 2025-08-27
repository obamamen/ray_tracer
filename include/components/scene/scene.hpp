// -----------------------------------------------------------------------------
//
//  ray_tracer - scene.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_SCENE
#define RAY_TRACER_SCENE
#include <vector>

#include "components/scene/object.hpp"

struct scene
{
    private:
    std::vector<object> objects;
    std::vector<material> materials;
};

#endif //RAY_TRACER_SCENE
