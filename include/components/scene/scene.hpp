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

    [[nodiscard]] std::span<old_object> get_all_objects()
    {
        return {objects};
    }

    color trace_ray(const ray& r, const int depth)
    {
        if(depth > render_settings::global_settings.max_bounces) return {.0f,.0f,.0f};

        float closest_t = 1e30f;
        const object* hit_obj = nullptr;
        vector3 hit_pos, hit_normal;

        for (const auto& obj : objects)
        {
            std::visit([&](auto const& shape) {
                if (intersection i; intersect(r, shape, i) && i.intersection_distance < closest_t)
                {
                    closest_t = i.intersection_distance;
                    hit_obj = &obj;
                    hit_pos = r.at(i.intersection_distance);
                    hit_normal = i.normal;
                }
            }, obj.shape);
        }

        if(!hit_obj)
            return {0.1f, 0.1f, 0.1f};

        return hit_obj->mat.albedo;
    }
};

#endif //RAY_TRACER_SCENE
