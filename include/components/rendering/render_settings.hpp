// -----------------------------------------------------------------------------
//
//  ray_tracer - render_settings.hpp
//
// -----------------------------------------------------------------------------

#ifndef RAY_TRACER_RENDER_SETTINGS
#define RAY_TRACER_RENDER_SETTINGS

enum class debug
{
    off,
    albedo,
    normal,
    depth
};

struct render_settings
{
    debug debug = debug::depth;
    bool multithreaded = true;
    bool cosine_hemisphere = true;

    int ssp = 10;
    int max_bounces = 10;
    static render_settings global_settings;
};

inline render_settings render_settings::global_settings{};

#endif //RAY_TRACER_RENDER_SETTINGS
