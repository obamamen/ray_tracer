#include "components/math/vector3.hpp"
#include "components/geometry/triangle.hpp"
#include "components/math/ray.hpp"
#include "components/rendering/color.hpp"
#include "components/rendering/material.hpp"
#include "components/rendering/texture.hpp"
#include "systems/math/random.hpp"
#include "systems/image/bmp.hpp"


#include <vector>
#include <cstdlib>
#include <atomic>
#include <mutex>
#include <cmath>
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

#include "components/rendering/camera.hpp"
#include "components/scene/scene.hpp"

int main()
{
    constexpr int width = 1920/4;
    constexpr int height = 1080/4;

    texture img(width, height);
    scene scene{};


    float s = 10;
    float d = 10;

    // Room corners
    vector3 p0(-s,-s,-s), p1(s,-s,-s), p2(s,s,-s), p3(-s,s,-s);
    vector3 p4(-s,-s,d), p5(s,-s,d), p6(s,s,d), p7(-s,s,d);

    // Reflectivity
    float main_r = 0.0f;
    float vertical_r = 0;

    color mainc = color(0.5,0.2,0.2);

    color cr = color(0.9,0.2,0.2);
    color cg = color(0.2,0.5,0.2);
    color cb = color(0.2,0.2,0.9);


    color mirrorc = color(0.85);


     // Floor (normal pointing up +Y)
     scene.add_object((object){triangle(p0,p1,p5), material(cb,0)});
     scene.add_object((object){triangle(p0,p5,p4), material(cb,0)});

     // Ceiling (normal pointing down -Y)
     scene.add_object((object){triangle(p3,p6,p2), material(mainc,0,(color){0.4})});
     scene.add_object((object){triangle(p3,p7,p6), material(mainc,0,(color){0.4})});

     // Left wall (normal pointing right +X)
     scene.add_object((object){triangle(p4,p7,p3), material(mirrorc,1)});
     scene.add_object((object){triangle(p4,p3,p0), material(mirrorc,1)});

     // Right wall (normal pointing left -X)
     scene.add_object((object){triangle(p1,p2,p6), material(cr,0)});
     scene.add_object((object){triangle(p1,p6,p5), material(cr,0)});

     // Back wall (normal pointing forward +Z)
     scene.add_object((object){triangle(p0,p3,p2), material(mirrorc,1)});
     scene.add_object((object){triangle(p0,p2,p1), material(mirrorc,1)});

     // Front wall (normal pointing backward -Z)
     scene.add_object((object){triangle(p4,p5,p6), material(cg, 0)});
     scene.add_object((object){triangle(p4,p6,p7), material(cg,0)});


    auto pillar_color = color(1);
    auto test = color(1.0);
    auto test_light = color(5);
    float pillar_ref = 1;

    int grid = 1;
    float grid_size = 6;
    for (int zoff = -grid; zoff <= grid; zoff += 1)
    {
        for (int xoff = -grid; xoff <= grid; xoff += 1)
            scene.add_object((object){sphere(vector3(xoff*grid_size, 0,zoff*grid_size),1),material(pillar_color,1)});
    }

    float l = d*(width/height)*0.9; // 0.95
    vector3 cam_pos(l,8,l);
    vector3 cam_look(0,0,0);
    vector3 cam_up(0,1,0);
    float aspect = float(width)/height;
    float fov = 0.5f;

    camera cam = camera(cam_pos, cam_look, cam_up,fov,aspect);
    cam.focus_dist = cam_pos.length();
    cam.lens_radius = 0;


    // Multithreading
    const int n_threads = static_cast<int>(1);
    std::vector<std::thread> threads;
    std::atomic<int> rows_done(0);
    std::mutex print_mutex;

    auto render_rows = [&](const int start, const int end)
    {
        random::set_seed(start);

        for(int y=start; y<end; y++)
        {
            for(int x=0; x<width; x++)
            {
                color pixel(0,0,0);
                for(int s_i=0; s_i<render_settings::global_settings.ssp; s_i++)
                {
                    float u = (x + .5f) / static_cast<float>(width);
                    float v = (y + .5f) / static_cast<float>(height);
                    pixel += scene.trace_ray(cam.generate_ray(u,v), 0 );
                }
                pixel /= float(render_settings::global_settings.ssp);
                color mapped(
                    (pixel.r),
                    (pixel.g),
                    (pixel.b)
                );
                color display(
                    std::pow(mapped.r, 1.0f/2.2f),
                    std::pow(mapped.g, 1.0f/2.2f),
                    std::pow(mapped.b, 1.0f/2.2f)
                );
                img.at(x,y) = display.clamped();
            }

            int done = ++rows_done;
        }
    };

    using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();

    int rows_per_thread = height / n_threads;
    for(int t=0; t<n_threads; t++)
    {
        int start = t * rows_per_thread;
        int end = (t == n_threads-1) ? height : start + rows_per_thread;
        threads.emplace_back(render_rows, start, end);
    }


    while(rows_done < height)
    {
        int done = rows_done.load();
        std::cerr << "\rRendering: " << 100.0 * done / height << "%" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    for(auto& th : threads) th.join();

    auto end_time = clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    int total_seconds = static_cast<int>(elapsed.count());
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;

    bmp::texture_to_bmp(img,"cube_pathtrace_final_normal.bmp");
    std::cout << "\nDone. Image saved as cube_pathtrace_final_blur.bmp\n";
    std::cout << "Render time: "
              << minutes << "m "
              << std::setw(2) << std::setfill('0') << seconds << "s\n";

    return 0;
}