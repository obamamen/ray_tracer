// -----------------------------------------------------------------------------
//  
//  ray_tracer - texture.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_TEXTURE
#define RAY_TRACER_TEXTURE
#include <cassert>
#include <cmath>

#include "color.hpp"

struct texture
{
    int width, height;
    color* pixels;

    // Constructor
    texture(int w, int h, const color& fill = color(0,0,0))
        : width(w), height(h), pixels(new color[w*h])
    {
        for (int i = 0; i < w * h; i++) pixels[i] = fill;
    }

    ~texture() {
        delete[] pixels;
    }

    texture(const texture& other)
        : width(other.width), height(other.height),
          pixels(new color[other.width * other.height])
    {
        for (int i = 0; i < width * height; i++)
            pixels[i] = other.pixels[i];
    }

    texture(texture&& other) noexcept
        : width(other.width), height(other.height), pixels(other.pixels)
    {
        other.width = 0;
        other.height = 0;
        other.pixels = nullptr;
    }

    texture& operator=(const texture& other) {
        if (this == &other) return *this;

        delete[] pixels;

        width = other.width;
        height = other.height;
        pixels = new color[width * height];

        for (int i = 0; i < width * height; i++)
            pixels[i] = other.pixels[i];

        return *this;
    }

    texture& operator=(texture&& other) noexcept {
        if (this == &other) return *this;

        delete[] pixels;

        width = other.width;
        height = other.height;
        pixels = other.pixels;

        other.width = 0;
        other.height = 0;
        other.pixels = nullptr;

        return *this;
    }

    [[nodiscard]] inline color& at(const int x, const int y) const {
        assert(x >= 0 && x < width && y >= 0 && y < height);
        return pixels[y * width + x];
    }

    [[nodiscard]] color sample(float u, float v) const {
        u = u - std::floor(u);
        v = v - std::floor(v);

        int x = static_cast<int>(u * static_cast<float>(width));
        int y = static_cast<int>(v * static_cast<float>(height));

        if (x >= width) x = width - 1;
        if (y >= height) y = height - 1;

        return at(x, y);
    }
};

#endif // RAY_TRACER_TEXTURE
