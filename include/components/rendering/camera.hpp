// -----------------------------------------------------------------------------
//
//  ray_tracer - camera.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_CAMERA
#define RAY_TRACER_CAMERA


#include <cmath>
#include <random>

#include "components/math/ray.hpp"
#include "components/math/vector3.hpp"
#include "systems/math/random.hpp"


struct camera
{
    vector3 pos;
    vector3 forward, right, up;
    float fov, aspect;
    float lens_radius = 0.0f;
    float focus_dist = 1.0f;

    camera(const vector3& p, const vector3& look_at, const vector3& up_vec, float fov_rad, float aspect_ratio)
        : pos(p), fov(fov_rad), aspect(aspect_ratio)
    {
        forward = (look_at - pos).normalized();
        right   = vector3::cross(forward, up_vec).normalized();
        up      = vector3::cross(right, forward).normalized();
    }

    ray generate_ray(const float u, const float v) const
    {
        float px = (2.0f * u - 1.0f) * fov * aspect;
        float py = (1.0f - 2.0f * v) * fov;
        vector3 dir = (forward + right * px + up * py).normalized();

        if(lens_radius > 0.0f)
        {
            float r1 = sqrt(randf());
            float r2 = 2.0f * M_PI * randf();
            float dx = r1 * cos(r2) * lens_radius;
            float dy = r1 * sin(r2) * lens_radius;

            vector3 origin = pos + right * dx + up * dy;
            vector3 focal_point = pos + dir * focus_dist;
            dir = (focal_point - origin).normalized();
            return ray(origin, dir);
        }
        return ray(pos, dir);
    }
};

#endif //RAY_TRACER_CAMERA
