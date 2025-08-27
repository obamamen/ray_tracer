// -----------------------------------------------------------------------------
//
//  ray_tracer - random.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_RANDOM
#define RAY_TRACER_RANDOM

#include <cmath>
#include <random>

#include "components/math/vector3.hpp"

inline float randf(std::mt19937& rng) { return std::uniform_real_distribution<float>(0.0f,1.0f)(rng); }

inline vector3 random_in_hemisphere(const vector3& normal, std::mt19937& rng)
{
    vector3 dir = vector3::random(1.0f).normalized();
    if (vector3::dot(dir, normal) < 0) dir = dir * -1.0f;
    return dir;
}

inline vector3 random_in_cone(const vector3& dir, float max_angle, std::mt19937& rng)
{
    float cos_theta = 1.0f - randf(rng) * (1.0f - std::cos(max_angle));
    float sin_theta = std::sqrt(1.0f - cos_theta*cos_theta);
    float phi = 2.0f * M_PI * randf(rng);

    vector3 w = dir.normalized();
    vector3 u = vector3::cross((std::fabs(w.x) > 0.1f ? vector3(0,1,0) : vector3(1,0,0)), w).normalized();
    vector3 v = vector3::cross(w, u);

    vector3 sample = u * (sin_theta * std::cos(phi)) +
                     v * (sin_theta * std::sin(phi)) +
                     w * cos_theta;
    return sample.normalized();
}

inline vector3 random_cosine_hemisphere(const vector3& normal, std::mt19937& rng)
{
    float r1 = randf(rng);
    float r2 = randf(rng);

    float phi = 2.0f * M_PI * r1;
    float cos_theta = std::sqrt(1.0f - r2);
    float sin_theta = std::sqrt(r2);

    float x = std::cos(phi) * sin_theta;
    float y = std::sin(phi) * sin_theta;
    float z = cos_theta;

    vector3 w = normal;
    vector3 u = vector3::cross((std::fabs(w.x) > 0.1f ? vector3(0,1,0) : vector3(1,0,0)), w).normalized();
    vector3 v = vector3::cross(w, u);

    return (u*x + v*y + w*z).normalized();
}

#endif //RAY_TRACER_RANDOM
