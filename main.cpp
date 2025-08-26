#include "components/math/vector3.hpp"
#include "components/geometry/triangle.hpp"
#include "components/math/ray.hpp"
#include "components/rendering/color.hpp"
#include "components/rendering/material.hpp"
#include "components/rendering/texture.hpp"
#include "components/math/random.hpp"

#include "systems/image/bmp.hpp"


#include <vector>
#include <cstdlib>
#include <atomic>
#include <mutex>
#include <cmath>
#include <iostream>
#include <random>
#include <thread>

// ----------------------------- helpers --------------------------------------
struct object
{
    triangle tri;
    material mat;
};

bool intersect_triangle(const ray& r, const triangle& tri, float& t_out, vector3& normal_out)
{
    const float EPSILON = 1e-4f;
    vector3 edge1 = tri.v1 - tri.v0;
    vector3 edge2 = tri.v2 - tri.v0;
    vector3 h = vector3::cross(r.direction, edge2);
    float a = vector3::dot(edge1, h);
    if (std::fabs(a) < EPSILON) return false;

    float f = 1.0f / a;
    vector3 s = r.origin - tri.v0;
    float u = f * vector3::dot(s, h);
    if (u < -EPSILON || u > 1.0f + EPSILON) return false;

    vector3 q = vector3::cross(s, edge1);
    float v = f * vector3::dot(r.direction, q);
    if (v < -EPSILON || (u + v) > 1.0f + EPSILON) return false;

    float t = f * vector3::dot(edge2, q);
    if (t >= EPSILON)
    {
        t_out = t;
        normal_out = vector3::cross(edge1, edge2).normalized();
        if (vector3::dot(normal_out, r.direction) > 0) return false;
        return true;
    }
    return false;
}

// ----------------------------- path tracer ----------------------------------
const int max_depth = 32;

color trace_ray(const ray& r, const std::vector<object>& scene, int depth, std::mt19937& rng)
{
    if(depth > max_depth) return color(0.0f,0.0f,0.0f);

    float closest_t = 1e30f;
    const object* hit_obj = nullptr;
    vector3 hit_pos, hit_normal;

    for(const auto& obj : scene)
    {
        float t;
        vector3 n;
        if(intersect_triangle(r, obj.tri, t, n) && t < closest_t)
        {
            closest_t = t;
            hit_obj = &obj;
            hit_pos = r.at(t);
            hit_normal = n;
        }
    }

    if(!hit_obj)
        return color(0.0f, 0.0f, 0.0f);

    const material& mat = hit_obj->mat;
    color emitted = mat.emission;
    color f = mat.albedo;
    float refl = mat.reflectivity;

    vector3 nl = vector3::dot(hit_normal, r.direction) < 0 ? hit_normal : -hit_normal;

    float p = std::max({f.r, f.g, f.b});
    if(depth > 5 && randf(rng) >= p) return emitted;
    if(depth > 5) f = f * (1.0f / p);

    if(std::max({f.r, f.g, f.b}) < 0.05f)
        return emitted;

    if(refl <= 0.0f)
    {
        vector3 dir = random_cosine_hemisphere(nl, rng);
        ray new_ray(hit_pos + nl*1e-4f, dir);
        return emitted + f * trace_ray(new_ray, scene, depth+1, rng);
    }
    else if(randf(rng) < refl)
    {
        vector3 refl_dir = r.direction - nl * 2.0f * vector3::dot(r.direction, nl);
        ray refl_ray(hit_pos + nl*1e-4f, refl_dir.normalized());
        return emitted + f * trace_ray(refl_ray, scene, depth+1, rng);
    }
    else
    {
        vector3 dir = random_cosine_hemisphere(nl, rng);
        ray diffuse_ray(hit_pos + nl*1e-4f, dir);
        return emitted + f * trace_ray(diffuse_ray, scene, depth+1, rng);
    }
}


// ----------------------------- main -----------------------------------------
int main()
{
    const int width = 2560;
    const int height = 2560;
    const int spp = 64;

    texture img(width, height);
    std::vector<object> scene;

    float s = 1.32;
    float d = 10;

    // Room corners
    vector3 p0(-s,-s,-s), p1(s,-s,-s), p2(s,s,-s), p3(-s,s,-s);
    vector3 p4(-s,-s,d), p5(s,-s,d), p6(s,s,d), p7(-s,s,d);

    // Reflectivity
    float main_r = 0.0f;
    float vertical_r = 0;

    // Floor
    scene.push_back({triangle(p0,p5,p1), material(color(0.6f,0.6f,0.6f),0)});
    scene.push_back({triangle(p0,p4,p5), material(color(0.6f,0.6f,0.6f),0)});

    // Ceiling light
    color ceil_light = color(0.8);
    scene.push_back({triangle(p3,p2,p6), material(color(0.6f),main_r)});
    scene.push_back({triangle(p3,p6,p7), material(color(0.6f),main_r)});

    // Left wall
    scene.push_back({triangle(p4,p3,p7), material(color(0.9),main_r)});
    scene.push_back({triangle(p4,p0,p3), material(color(0.9),main_r)});

    // Right wall
    scene.push_back({triangle(p1,p6,p2), material(color(0.6),main_r)});
    scene.push_back({triangle(p1,p5,p6), material(color(0.6),main_r)});

    // Back wall
    scene.push_back({triangle(p0,p2,p3), material(color(0.6f),1)});
    scene.push_back({triangle(p0,p1,p2), material(color(0.6f),1)});

    // Front wall
    scene.push_back({triangle(p4,p6,p5), material(color(0.9f),0)});
    scene.push_back({triangle(p4,p7,p6), material(color(0.9f),0)});

    // ---- Add a central pillar ----
    float pillar_size = 0.4f;
    vector3 c0(-pillar_size,-pillar_size,2), c1(pillar_size,-pillar_size,2);
    vector3 c2(pillar_size,pillar_size,2), c3(-pillar_size,pillar_size,2);
    vector3 c4(-pillar_size,-pillar_size,5), c5(pillar_size,-pillar_size,5);
    vector3 c6(pillar_size,pillar_size,5), c7(-pillar_size,pillar_size,5);

    color pillar_color = color(0.8,0.4,0.4);
    color test = color(1.0);
    color test_light = color(1);
    float pillar_ref = .0;

    // Bottom face (pointing downward)
    scene.push_back({triangle(c0,c2,c1), material(pillar_color,pillar_ref,(color){25})});
    scene.push_back({triangle(c0,c3,c2), material(pillar_color,pillar_ref,(color){25})});

    // Top face (pointing upward)
    scene.push_back({triangle(c4,c5,c6), material(pillar_color,pillar_ref)});
    scene.push_back({triangle(c4,c6,c7), material(pillar_color,pillar_ref)});

    // Front face (toward positive z)
    scene.push_back({triangle(c0,c5,c4), material(pillar_color,pillar_ref)});
    scene.push_back({triangle(c0,c1,c5), material(pillar_color,pillar_ref)});

    // Back face (toward negative z)
    scene.push_back({triangle(c3,c6,c2), material(pillar_color,pillar_ref)});
    scene.push_back({triangle(c3,c7,c6), material(pillar_color,pillar_ref)});

    // Left face (toward negative x)
    scene.push_back({triangle(c0,c7,c3), material(pillar_color,pillar_ref)});
    scene.push_back({triangle(c0,c4,c7), material(pillar_color,pillar_ref)});

    // Right face (toward positive x)
    scene.push_back({triangle(c1,c6,c5), material(pillar_color,pillar_ref)});
    scene.push_back({triangle(c1,c2,c6), material(pillar_color,pillar_ref)});


    vector3 cam_pos(3,0,(1+d)*1.35);
    vector3 cam_look(0,0,d/1.35);
    vector3 cam_up(0,1,0);
    float aspect = float(width)/height;
    float fov = 0.5f;

    vector3 forward = (cam_look - cam_pos).normalized();
    vector3 right   = vector3::cross(forward, cam_up).normalized();
    vector3 up      = vector3::cross(right, forward).normalized();

    // Multithreading
    const int n_threads = (int)std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::atomic<int> rows_done(0);
    std::mutex print_mutex;

    auto render_rows = [&](int start, int end)
    {
        std::mt19937 rng(start);

        float lens_radius = 0.0f;
        float focus_dist = 2.0f;

        for(int y=start; y<end; y++)
        {
            for(int x=0; x<width; x++)
            {
                color pixel(0,0,0);
                for(int s_i=0; s_i<spp; s_i++)
                {
                    // Generate normalized pixel coordinates
                    float u = ((x + randf(rng)) / width * 2 - 1) * fov * aspect;
                    float v = (1 - (y + randf(rng)) / height * 2) * fov;

                    // --- Depth of Field ---
                    // Sample point on lens disk
                    float r1 = sqrt(randf(rng));
                    float r2 = 2.0f * M_PI * randf(rng);
                    float dx = r1 * cos(r2) * lens_radius;
                    float dy = r1 * sin(r2) * lens_radius;

                    // Offset camera origin
                    vector3 origin = cam_pos + right * dx + up * dy;

                    // Compute focal point on focal plane
                    vector3 focal_point = cam_pos + (forward + right * u + up * v).normalized() * focus_dist;

                    // New ray direction through focal point
                    vector3 dir = (focal_point - origin).normalized();

                    ray r(origin, dir);
                    pixel += trace_ray(r, scene, 0, rng);
                }
                pixel /= float(spp);
                //pixel *= 0.8f;
                // color display(
                //     std::pow(pixel.r, 1.0f/2.2f),
                //     std::pow(pixel.g, 1.0f/2.2f),
                //     std::pow(pixel.b, 1.0f/2.2f)
                // );


                img.at(x,y) = pixel.clamped();
            }

            int done = ++rows_done;
            if (done % 1 == 0) {
                std::lock_guard<std::mutex> lock(print_mutex);
                std::cerr << "\r                                                                      ";
                std::cerr << "\rRendering: " << 100.0 * done / height << "%" << std::flush;
            }
        }
    };

    int rows_per_thread = height / n_threads;
    for(int t=0; t<n_threads; t++)
    {
        int start = t * rows_per_thread;
        int end = (t == n_threads-1) ? height : start + rows_per_thread;
        threads.emplace_back(render_rows, start, end);
    }

    for(auto& th : threads) th.join();

    bmp::texture_to_bmp(img,"cube_pathtrace_final.bmp");
    std::cout << "\nDone. Image saved as cube_pathtrace_final.bmp\n";

    return 0;
}