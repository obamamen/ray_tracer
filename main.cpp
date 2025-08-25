#include "components/math/vector3.hpp"
#include "components/geometry/triangle.hpp"
#include "components/math/ray.hpp"
#include "components/rendering/color.hpp"
#include "components/rendering/material.hpp"
#include "components/rendering/texture.hpp"
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

inline float randf(std::mt19937& rng) { return std::uniform_real_distribution<float>(0.0f,1.0f)(rng); }

vector3 random_in_hemisphere(const vector3& normal, std::mt19937& rng)
{
    vector3 dir = vector3::random(1.0f).normalized();
    if (vector3::dot(dir, normal) < 0) dir = dir * -1.0f;
    return dir;
}

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
const int max_depth = 48;

color trace_ray(const ray& r, const std::vector<object>& scene, int depth, std::mt19937& rng)
{
    if(depth > max_depth) return color(0.1,0.1,0.1);

    float closest_t = 1e30f;
    const material* hit_mat = nullptr;
    vector3 hit_pos, hit_normal;

    for(const auto& obj : scene)
    {
        float t;
        vector3 n;
        if(intersect_triangle(r, obj.tri, t, n) && t < closest_t)
        {
            closest_t = t;
            hit_mat = &obj.mat;
            hit_pos = r.at(t);
            hit_normal = n;
        }
    }

    if(!hit_mat) return color(0,0,0);

    color emitted = hit_mat->emission;
    float refl = hit_mat->reflectivity;


    if(refl <= 0.0f)
    {
        // pure diffuse
        vector3 diffuse_dir = random_in_hemisphere(hit_normal, rng);
        ray diffuse_ray(hit_pos + hit_normal*1e-4f, diffuse_dir);
        return emitted + hit_mat->albedo * trace_ray(diffuse_ray, scene, depth+1, rng);
    }

    if(depth < 2 && randf(rng) < refl) {
        // specular
        vector3 refl_dir = r.direction - hit_normal * 2.0f * vector3::dot(r.direction, hit_normal);
        ray refl_ray(hit_pos + hit_normal*1e-4f, refl_dir.normalized());
        return emitted + hit_mat->albedo * trace_ray(refl_ray, scene, depth+1, rng);
    } else {
        // diffuse
        vector3 diffuse_dir = random_in_hemisphere(hit_normal, rng);
        ray diffuse_ray(hit_pos + hit_normal*1e-4f, diffuse_dir);
        return emitted + hit_mat->albedo * trace_ray(diffuse_ray, scene, depth+1, rng);
    }
}

// ----------------------------- main -----------------------------------------
int main()
{
    const int width = 1024;
    const int height = 1024;
    const int spp = 1024;

    texture img(width, height);
    std::vector<object> scene;

    float s = 1.0f;
    float d = 12.0f;

    vector3 p0(-s,-s,-s), p1(s,-s,-s), p2(s,s,-s), p3(-s,s,-s);
    vector3 p4(-s,-s,d), p5(s,-s,d), p6(s,s,d), p7(-s,s,d);

    float main_r = 0.0f;
    float vertical_r = 0.0f;

    // Floor
    scene.push_back({triangle(p0,p5,p1), material(color(0.8f,0.8f,0.8f),vertical_r)});
    scene.push_back({triangle(p0,p4,p5), material(color(0.8f,0.8f,0.8f),vertical_r)});

    // Ceiling
    scene.push_back({triangle(p3,p2,p6), material(color(0.8f,0.8f,0.8f),vertical_r,color(0.4f,0.4f,0.4f))});
    scene.push_back({triangle(p3,p6,p7), material(color(0.8f,0.8f,0.8f),vertical_r,color(0.4f,0.4f,0.4f))});

    // Left wall
    scene.push_back({triangle(p4,p3,p7), material(color(0.8f,0.5f,0.4f),main_r)});
    scene.push_back({triangle(p4,p0,p3), material(color(0.8f,0.5f,0.4f),main_r)});

    // Right wall
    scene.push_back({triangle(p1,p6,p2), material(color(0.4f,0.5f,0.8f),main_r)});
    scene.push_back({triangle(p1,p5,p6), material(color(0.4f,0.5f,0.8f),main_r)});

    // Back wall
    scene.push_back({triangle(p0,p2,p3), material(color(0.8f,0.8f,0.8f),0)});
    scene.push_back({triangle(p0,p1,p2), material(color(0.8f,0.8f,0.8f),0)});

    // Front wall
    scene.push_back({triangle(p4,p6,p5), material(color(0.8f,0.8f,0.8f),0.5,color(0.9f,0.9f,0.9f))});
    scene.push_back({triangle(p4,p7,p6), material(color(0.8f,0.8f,0.8f),0.5,color(0.9f,0.9f,0.9f))});


    vector3 cam_pos(0,0,d);
    vector3 cam_look(0,0,0);
    vector3 cam_up(0,1,0);
    float aspect = float(width)/height;
    float fov = 0.5f;

    vector3 forward = (cam_look - cam_pos).normalized();
    vector3 right   = vector3::cross(forward, cam_up).normalized();
    vector3 up      = vector3::cross(right, forward).normalized();

    // Multithreading
    int n_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::atomic<int> rows_done(0);
    std::mutex print_mutex;

    auto render_rows = [&](int start, int end)
    {
        std::mt19937 rng(start);
        for(int y=start; y<end; y++)
        {
            for(int x=0; x<width; x++)
            {
                color pixel(0,0,0);
                for(int s_i=0; s_i<spp; s_i++)
                {
                    float u = ((x + randf(rng)) / width * 2 - 1) * fov * aspect;
                    float v = (1 - (y + randf(rng)) / height * 2) * fov;
                    vector3 dir = (forward + right * u + up * v).normalized();
                    ray r(cam_pos, dir);
                    pixel += trace_ray(r, scene, 0, rng);
                }
                img.at(x,y) = (pixel / float(spp)).clamped();
            }

            int done = ++rows_done;
            if (done % 1 == 0) {
                std::lock_guard<std::mutex> lock(print_mutex);
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