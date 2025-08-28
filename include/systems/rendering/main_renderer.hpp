// -----------------------------------------------------------------------------
//
//  ray_tracer - main_renderer.hpp
//
// -----------------------------------------------------------------------------

#ifndef RAY_TRACER_MAIN_RENDERER
#define RAY_TRACER_MAIN_RENDERER

#include "components/math/ray.hpp"
#include "components/rendering/color.hpp"
#include "components/scene/object.hpp"
#include "systems/math/random.hpp"

// color trace_ray(const ray& r, const std::vector<object>& scene, int depth)
// {
//     if(depth > max_depth) return color(.0f,.0f,.0f);
//
//     float closest_t = 1e30f;
//     const object* hit_obj = nullptr;
//     vector3 hit_pos, hit_normal;
//
//     for(const auto& obj : scene)
//     {
//         float t;
//         vector3 n;
//         if(intersect_triangle(r, obj.tri, t, n) && t < closest_t)
//         {
//             closest_t = t;
//             hit_obj = &obj;
//             hit_pos = r.at(t);
//             hit_normal = n;
//         }
//     }
//
//     if(!hit_obj)
//         return color(0.1f, 0.1f, 0.1f);
//
//     const material& mat = hit_obj->mat;
//     color emitted = mat.emission;
//     color f = mat.albedo;
//     float refl = mat.reflectivity;
//
//     vector3 nl = vector3::dot(hit_normal, r.direction) < 0 ? hit_normal : -hit_normal;
//
//     float p = std::max({f.r, f.g, f.b});
//     if(depth > 4 && randf() >= p) return emitted;
//     if(depth > 4) f = f * (1.0f / p);
//
//     if(std::max({f.r, f.g, f.b}) < 0.05f)
//         return emitted;
//
//     if(refl <= 0.0f)
//     {
//         vector3 dir = random_cosine_hemisphere(nl);
//         ray new_ray(hit_pos + nl*1e-4f, dir);
//         return emitted + f * trace_ray(new_ray, scene, depth+1);
//     }
//     if(randf() < refl)
//     {
//         vector3 refl_dir = r.direction - nl * 2.0f * vector3::dot(r.direction, nl);
//         ray refl_ray(hit_pos + nl*1e-4f, refl_dir.normalized());
//         return emitted + f * trace_ray(refl_ray, scene, depth+1);
//     }
//     {
//         vector3 dir = random_cosine_hemisphere(nl);
//         ray diffuse_ray(hit_pos + nl*1e-4f, dir);
//         return emitted + f * trace_ray(diffuse_ray, scene, depth+1);
//     }
// }

#endif//RAY_TRACER_MAIN_RENDERER
