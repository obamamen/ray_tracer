// -----------------------------------------------------------------------------
//
//  ray_tracer - scene.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_SCENE
#define RAY_TRACER_SCENE
#include <memory>
#include <span>
#include <vector>

#include "components/math/ray.hpp"
#include "components/rendering/render_settings.hpp"
#include "components/scene/object.hpp"
#include "systems/math/intersection.hpp"


struct environment
{
    color fog_color = color(0, 0, 0);
    float fog_density = 0.01;
    color sky_color = color(0.1, 0.1, 0.2);
};

struct scene
{
public:
    environment environment;
private:
    std::vector<object> objects;

public:
    size_t add_object(const object& o)
    {
        objects.push_back(o);
        return objects.size() - 1;
    }

    [[nodiscard]] size_t object_count() const
    {
        return objects.size();
    }

    [[nodiscard]] object* unsafe_get_all_objects()
    {
        return objects.data();
    }

    [[nodiscard]] std::span<object> get_all_objects()
    {
        return {objects};
    }

    color trace_ray(const ray& r, const int depth)
    {
        constexpr float EPSILON = 1e-6f;
        if(depth > render_settings::global_settings.max_bounces) return {.0f,.0f,.0f};

        float closest_t = 1e30f;
        const object* hit_obj = nullptr;
        vector3 hit_pos, hit_normal;
        intersection is;

        for (const auto& obj : objects)
        {
            std::visit([&](auto const& shape) {
                if (intersection i; intersect(r, shape, i) && i.intersection_distance < closest_t)
                {
                    closest_t = i.intersection_distance;
                    hit_obj = &obj;
                    hit_pos = r.at(i.intersection_distance);
                    hit_normal = i.normal;
                    is = i;
                }
            }, obj.shape);
        }

        if(!hit_obj)
            return {.0f, .0f, .0f};


        // debugs
        if (render_settings::global_settings.debug == debug::albedo)
            return hit_obj->mat.albedo;

        if (render_settings::global_settings.debug == debug::normal)
            return {hit_normal.x, hit_normal.y, hit_normal.z};

        if (render_settings::global_settings.debug == debug::depth)
        {
            float depth_linear = (closest_t / 1024.0f);
            float depth_non_linear = std::sqrt(depth_linear);
            return (color){depth_non_linear};
        }


        const material& mat = hit_obj->mat;
        color emitted = mat.emission;
        color f = mat.albedo;
        float refl = mat.reflectivity;

        vector3 nl = hit_normal;
        if (vector3::dot(hit_normal, r.direction) > 0.0f)
            nl = -hit_normal;

        float p = std::max({f.r, f.g, f.b});
        if(depth > 4 && randf() >= p) return emitted;
        if(depth > 4) f = f * (1.0f / p);

        if(std::max({f.r, f.g, f.b}) < 0.05f)
            return emitted;

        if(refl <= 0.0f)
        {
            vector3 dir = random_cosine_hemisphere(nl);
            ray new_ray(hit_pos + nl*1e-4f, dir);
            return emitted + f * trace_ray(new_ray, depth+1);
        }
        if(randf() < refl)
        {
            vector3 refl_dir = r.direction - nl * 2.0f * vector3::dot(r.direction, nl);
            ray refl_ray(hit_pos + nl*1e-4f, refl_dir.normalized());
            return emitted + f * trace_ray(refl_ray, depth+1);
        }
        {
            vector3 dir = random_cosine_hemisphere(nl);
            ray diffuse_ray(hit_pos + nl*1e-4f, dir);
            return emitted + f * trace_ray(diffuse_ray, depth+1);
        }
    }
};

#endif //RAY_TRACER_SCENE
