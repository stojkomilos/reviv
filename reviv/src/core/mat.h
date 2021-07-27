#pragma once

#include"rv_pch.hpp"

using std::cin; using std::cout; using std::endl;

namespace mat{

    class Vec1f
    {
    public:
        float x;
        Vec1f() = default;
        Vec1f(float x);
        Vec1f(const Vec1f&) = default;
    };

    class Vec2f
    {
    public:
        float x, y;
        Vec2f() = default;
        Vec2f(float x, float y);
        Vec2f(const Vec2f&) = default;
    };

    class Vec3f
    {
    public:
        float a[3];
        Vec3f() = default;
        Vec3f(float x, float y, float z);
        Vec3f(const Vec3f&) = default;
    };
    Vec3f& operator+=(Vec3f& first, const Vec3f& second);
    Vec3f operator-(const Vec3f& first, const Vec3f second);
    Vec3f operator-(const Vec3f& first);
    Vec3f operator/(const Vec3f& vec, float scalar);

    class Vec4f
    {
    public:
        float a[4];
        Vec4f() = default;
        Vec4f(float x, float y, float z, float w);
        Vec4f(const Vec4f&) = default;
        Vec4f(const Vec3f& vec, float scalar);
    };

    class Mat3
    {
    public:
        float a[3][3];
        Mat3() = default;
        Mat3(float n);
        Mat3(const Mat3&) = default;
    };

    class Mat4
    {
    public:
        float a[4][4];
        Mat4() = default;
        Mat4(float n);
        Mat4(const Mat4&) = default;
    };

    class Vec1i
    {
    public:
        int x;
        Vec1i() = default;
        Vec1i(int x);
        Vec1i(const Vec1i&) = default;
    };

    class Vec2i
    {
    public:
        int x, y;
        Vec2i() = default;
        Vec2i(int x, int y);
        Vec2i(const Vec2i&) = default;
    };

    class Vec3i
    {
    public:
        int x, y, z;
        Vec3i() = default;
        Vec3i(int x, int y, int z);
        Vec3i(const Vec3i&) = default;
    };

    class Vec4i
    {
    public:
        int x, y, z, w;
        Vec4i() = default;
        Vec4i(int x, int y, int z, int w);
        Vec4i(const Vec4i&) = default;
    };
    
    struct Quaternion
    {
        float x[4];
    };

/*
    Quaternion conjugate(Quaternion quaternion);
    float moduleSquared(Quaternion quaternion);
    Quaternion inverse(Quaternion quaternion);
    Quaternion operator*(Quaternion quaternion, float scalar);
*/

    Vec3f operator*(const Vec3f& thing, const float& scalar);
    Vec3f operator*(const float& scalar, const Vec3f& thing);
    Vec4f operator/(const Vec4f& thing, const float& scalar);
    Vec4f operator/(const float& scalar, const Vec4f& thing);

    Mat4 translate(Mat4 mtx, const Vec4f& vec);
    Mat4 scale(Mat4 a, const Vec4f& b);
    Mat4 rotateX(float theta); // supposed to be roll
    Mat4 rotateY(float theta); // supposed to be pitch
    Mat4 rotateZ(float theta); // supposed to be yaw

    Mat4 multiply(const Mat4& first, const Mat4& second);
    Vec4f multiply(const Mat4& a, const Vec4f& b);

    float module(const Vec3f& a);
    float module(const Vec2f& a);
    float dot(const Vec3f& a, const Vec3f& b);
    float dot(const Vec2f& a, const Vec2f& b);
    Vec3f projection(const Vec3f& a, const Vec3f& b);
    Vec3f normalise(Vec3f a);
    Vec2f normalise(Vec2f a);
    Vec3f cross(const Vec3f& a, const Vec3f& b);
    Vec3f multiplyScalar(Vec3f a, float b);
    Vec3f multiplyScalar(Vec3f a, int b);
    Vec3f add(const Vec3f& a, const Vec3f& b);
    Vec3f subtract(const Vec3f& a, const Vec3f& b);
    Vec2f subtract(const Vec2f& a, const Vec2f& b);
    //int sgn(int n);
    //int sgn(float n);
    //float clampTwoSide(float minPossibleValue, float maxPossibleValue, float n);
    //float clampMin(float minPossibleValue, float n);
    //float clampMax(float maxPossibleValue, float n);

    class Rotation
    {
    public:
        float pitch, yaw, roll;

        Rotation() = default;
        Rotation(const Rotation&) = default;
        Rotation(const Vec3f& initRotation) 
            : pitch(initRotation.a[0]), yaw(initRotation.a[1]), roll(initRotation.a[2]) {}
    };

    Vec3f getDirectionFromRotation(const Rotation& rotation);
    float degreesToRadians(float angleInDegrees);
    float radiansToDegrees(float angleInRadians);

}

using namespace mat;

extern const Mat4 identity;

void log(const Vec1f& thing);
void log(const Vec2f& thing);
void log(const Vec3f& thing);
void log(const Vec4f& thing);

void log(const Vec1i& thing);
void log(const Vec2i& thing);
void log(const Vec3i& thing);
void log(const Vec4i& thing);

void log(const Mat3& thing);
void log(const Mat4& thing);

//void log(const bool& thing);
void log(const Rotation& thing);