// -----------------------------------------------------------------------------
//  
//  ray_tracer - vector3.hpp
//
// -----------------------------------------------------------------------------



#ifndef RAY_TRACER_VECTOR3
#define RAY_TRACER_VECTOR3

#include <cmath>

struct vector3
{
    float x, y, z;


    vector3() : x(0), y(0), z(0) {}
    vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}


    vector3 operator+(const vector3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    vector3 operator-(const vector3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    vector3 operator*(const float t) const { return {x * t, y * t, z * t}; }
    vector3 operator/(const float t) const { return {x / t, y / t, z / t}; }
    vector3& operator+=(const vector3& o) { x += o.x; y += o.y; z += o.z; return *this; }
    vector3& operator*=(const float t) { x *= t; y *= t; z *= t; return *this; }
    void normalize() { (*this) = this->normalized(); }


    [[nodiscard]] float length() const { return std::sqrt(x*x + y*y + z*z); }
    [[nodiscard]] float length_2() const { return x*x + y*y + z*z; } // squared length
    [[nodiscard]] vector3 normalized() const { const float len = length(); return *this / len; }


    static float dot(const vector3& a, const vector3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
    static vector3 cross(const vector3& a, const vector3& b)
    {
        return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
    }
    static vector3 reflect(const vector3& v, const vector3& n) { return v - n * (2.0f * dot(v,n)); }
    static vector3 lerp(const vector3& a, const vector3& b, float t) { return a*(1-t) + b*t; }
    static float distance(const vector3& a, const vector3& b) { return (a-b).length(); }
    static float distance_2(const vector3& a, const vector3& b) { return (a-b).length_2(); }
    static vector3 min(const vector3& a, const vector3& b)
    {
        return { a.x<b.x?a.x:b.x, a.y<b.y?a.y:b.y, a.z<b.z?a.z:b.z };
    }
    static vector3 max(const vector3& a, const vector3& b)
    {
        return { a.x>b.x?a.x:b.x, a.y>b.y?a.y:b.y, a.z>b.z?a.z:b.z };
    }
    static vector3 random(const float radius = 1.0f)
    {
        const float u = static_cast<float>(rand()) / RAND_MAX;
        const float v = static_cast<float>(rand()) / RAND_MAX;
        const float theta = u * 2.0f * static_cast<float>(M_PI);
        const float phi = std::acos(2.0f*v - 1.0f);
        const float r = std::cbrt(static_cast<float>(rand()) / RAND_MAX) * radius;

        const float sinPhi = std::sin(phi);
        return {
            r * sinPhi * std::cos(theta),
            r * sinPhi * std::sin(theta),
            r * std::cos(phi)
        };
    }

}; // vector3



#endif // RAY_TRACER_VECTOR3
