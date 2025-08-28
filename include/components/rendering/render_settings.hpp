// -----------------------------------------------------------------------------
//
//  ray_tracer - render_settings.hpp
//
// -----------------------------------------------------------------------------

#ifndef RAY_TRACER_RENDER_SETTINGS
#define RAY_TRACER_RENDER_SETTINGS

struct render_settings
{
    bool only_albedo = false;
    bool multithreaded = true;
    bool cosine_hemisphere = true;

    int ssp = 256;
    int max_bounces = 16;

    static inline render_settings global_settings{};
};

#endif //RAY_TRACER_RENDER_SETTINGS
