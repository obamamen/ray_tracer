#include <cstdint>
#include <iostream>
#include <fstream>


#include "rendering/texture.hpp"


struct rand_s
{
    uint64_t state = 1ULL;

    uint64_t get_next()
    {
        state = state * 877153429960259ULL;
        state = (state >> 32) ^ (state >> 24);
        return state ^ (state >> 16);
    }

    float get_float()
    {
        return static_cast<float>(get_next() & 0xFFFFFF) / static_cast<float>(0x1000000);
    }
};

bool save_bmp(const texture& tex, const char* filename);

int main()
{
    texture texture(1000,1000);
    rand_s rand;
    for (int x = 0; x < texture.width; x++)
    {
        for (int y = 0; y < texture.height; y++)
        {
            // texture.at(x, y) = color(
            //     static_cast<float>(x) / texture.width,
            //     static_cast<float>(y) / texture.height,
            //     0.0f
            // );
            texture.at(x,y) = color(rand.get_float(), 0, 0);
        }
    }
    save_bmp(texture, "output.bmp");
    return 0;
}

bool save_bmp(const texture& tex, const char* filename) // chatgpt
{
    const int w = tex.width;
    const int h = tex.height;

    const int row_padding = (4 - (w * 3) % 4) % 4; // each row must be multiple of 4 bytes
    const int filesize = 54 + (3*w + row_padding) * h;

    unsigned char file_header[14] = {
        'B','M',  // Signature
        0,0,0,0,  // File size (will fill later)
        0,0,      // Reserved
        0,0,      // Reserved
        54,0,0,0  // Data offset
    };

    unsigned char info_header[40] = {
        40,0,0,0,   // Header size
        0,0,0,0,    // Width (will fill)
        0,0,0,0,    // Height (will fill)
        1,0,        // Planes
        24,0,       // Bits per pixel
        0,0,0,0,    // Compression
        0,0,0,0,    // Image size
        0,0,0,0,    // X pixels per meter
        0,0,0,0,    // Y pixels per meter
        0,0,0,0,    // Total colors
        0,0,0,0     // Important colors
    };

    // Fill file size
    file_header[2] = (unsigned char)(filesize);
    file_header[3] = (unsigned char)(filesize >> 8);
    file_header[4] = (unsigned char)(filesize >> 16);
    file_header[5] = (unsigned char)(filesize >> 24);

    // Fill width
    info_header[4] = (unsigned char)(w);
    info_header[5] = (unsigned char)(w >> 8);
    info_header[6] = (unsigned char)(w >> 16);
    info_header[7] = (unsigned char)(w >> 24);

    // Fill height
    info_header[8] = (unsigned char)(h);
    info_header[9] = (unsigned char)(h >> 8);
    info_header[10] = (unsigned char)(h >> 16);
    info_header[11] = (unsigned char)(h >> 24);

    std::ofstream out(filename, std::ios::binary);
    if (!out) return false;

    out.write((char*)file_header, 14);
    out.write((char*)info_header, 40);
xdekwcjsimzr hau,
    unsigned char pad[3] = {0,0,0};

    // BMP rows are bottom-to-top
    for (int y = h - 1; y >= 0; y--) {
        for (int x = 0; x < w; x++) {
            const color& c = tex.at(x,y);
            unsigned char r = static_cast<unsigned char>(std::min(c.r,1.0f)*255.0f);
            unsigned char g = static_cast<unsigned char>(std::min(c.g,1.0f)*255.0f);
            unsigned char b = static_cast<unsigned char>(std::min(c.b,1.0f)*255.0f);
            out.put(b);
            out.put(g);
            out.put(r);
        }
        out.write((char*)pad, row_padding);
    }

    return true;
}
