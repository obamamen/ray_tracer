// -----------------------------------------------------------------------------
//  
//  ray_tracer - bmp.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_BMP
#define RAY_TRACER_BMP

#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <stdexcept>
#include <vector>

#include "components/rendering/texture.hpp"

struct bmp
{
private:
#pragma pack(push, 1)
    struct bmp_file_header
    {
        uint16_t bfType{0x4D42};   // "BM"
        uint32_t bfSize{0};        // File size in bytes
        uint16_t bfReserved1{0};
        uint16_t bfReserved2{0};
        uint32_t bfOffBits{54};    // Offset to pixel array
    };

    struct bmp_info_header
    {
        uint32_t biSize{40};
        int32_t  biWidth{0};
        int32_t  biHeight{0};
        uint16_t biPlanes{1};
        uint16_t biBitCount{24};
        uint32_t biCompression{0};
        uint32_t biSizeImage{0};
        int32_t  biXPelsPerMeter{2835};
        int32_t  biYPelsPerMeter{2835};
        uint32_t biClrUsed{0};
        uint32_t biClrImportant{0};
    };
#pragma pack(pop)
public:
    static bool texture_to_bmp(const texture& tex, const char* filename)
    {
        int width  = tex.width;
        int height = tex.height;

        int row_padded = (width * 3 + 3) & ~3;
        int data_size  = row_padded * height;

        bmp_file_header fileHeader;
        bmp_info_header infoHeader;

        fileHeader.bfSize    = sizeof(bmp_file_header) + sizeof(bmp_info_header) + data_size;
        fileHeader.bfOffBits = sizeof(bmp_file_header) + sizeof(bmp_info_header);

        infoHeader.biWidth      = width;
        infoHeader.biHeight     = height;
        infoHeader.biSizeImage  = data_size;

        std::ofstream file(filename, std::ios::binary);
        if (!file) return false;

        file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
        file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

        std::vector<uint8_t> row(row_padded);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                const color& c = tex.at(x, height - y - 1); // flip vertically

                uint8_t r = static_cast<uint8_t>(std::clamp(c.r, 0.0f, 1.0f) * 255.0f);
                uint8_t g = static_cast<uint8_t>(std::clamp(c.g, 0.0f, 1.0f) * 255.0f);
                uint8_t b = static_cast<uint8_t>(std::clamp(c.b, 0.0f, 1.0f) * 255.0f);

                row[x * 3 + 0] = b;
                row[x * 3 + 1] = g;
                row[x * 3 + 2] = r;
            }
            // zero out padding
            for (int i = width * 3; i < row_padded; i++)
                row[i] = 0;

            file.write(reinterpret_cast<char*>(row.data()), row_padded);
        }

        return true;
    }

    static texture bmp_to_texture(const char* filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file)
            throw std::runtime_error("Failed to open BMP file");

        bmp_file_header fileHeader;
        bmp_info_header infoHeader;

        file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
        file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

        if (fileHeader.bfType != 0x4D42)
            throw std::runtime_error("Not a BMP file");

        if (infoHeader.biBitCount != 24 || infoHeader.biCompression != 0)
            throw std::runtime_error("Only uncompressed 24-bit BMP supported");

        texture tex(infoHeader.biWidth, infoHeader.biHeight);

        file.seekg(fileHeader.bfOffBits, std::ios::beg);

        int row_padded = (infoHeader.biWidth * 3 + 3) & ~3;
        std::vector<uint8_t> row(row_padded);

        for (int y = 0; y < infoHeader.biHeight; y++)
        {
            file.read(reinterpret_cast<char*>(row.data()), row_padded);
            for (int x = 0; x < infoHeader.biWidth; x++)
            {
                uint8_t b = row[x * 3 + 0];
                uint8_t g = row[x * 3 + 1];
                uint8_t r = row[x * 3 + 2];

                tex.at(x, infoHeader.biHeight - y - 1) = color(
                    r / 255.0f,
                    g / 255.0f,
                    b / 255.0f
                );
            }
        }

        return tex;
    }
};

#endif // RAY_TRACER_BMP
