#pragma once

#include"rv_pch.hpp"

using std::cin; using std::cout; using std::endl;

namespace mat{

    class Vec1f
    {
    public:
        float a[1];
        Vec1f() = default;
        Vec1f(float x);
        Vec1f(const Vec1f&) = default;
    };

    class Vec2f
    {
    public:
        float a[2];
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

    // NOT TESTED
    bool checkIfPointBelongsToLine(const Vec3f& linePoint1, const Vec3f& linePoint2, const Vec3f& point);

    // NOT TESTED
    float getDistancePointLine(const Vec3f& point, const Vec3f& lineA, const Vec3f& lineB);

    inline bool sign(float a)
    { 
        if(a>0) 
            return true; 
        return false; 
    }

    Mat4 translate(Mat4 mtx, const Vec4f& vec);
    Mat4 scale(Mat4 mtx, const Vec4f& vec);
    Mat4 rotateX(float theta); // supposed to be roll
    Mat4 rotateY(float theta); // supposed to be pitch
    Mat4 rotateZ(float theta); // supposed to be yaw

    Mat4 multiply(const Mat4& first, const Mat4& second);
    Vec4f multiply(const Mat4& mtx, const Vec4f& vec);

    float module(const Vec4f& vec);
    float module(const Vec3f& vec);
    float module(const Vec2f& vec);
    float module(const Vec1f& vec);

    float dot(const Vec3f& first, const Vec3f& second);
    float dot(const Vec2f& first, const Vec2f& second);
    Vec3f projection(const Vec3f& first, const Vec3f& second);

    Vec2f subtract(const Vec2f& first, const Vec2f& second);
    Vec2f operator-(const Vec2f& first, const Vec2f& second);

    Vec3f cross(const Vec3f& first, const Vec3f& second);
    Vec3f multiplyScalar(Vec3f vec, float scalar);
    Vec3f multiplyScalar(Vec3f vec, int intiger);
    Vec3f add(const Vec3f& a, const Vec3f& b);
    Vec3f subtract(const Vec3f& first, const Vec3f& second);

    Vec3f& operator+=(Vec3f& first, const Vec3f& second);
    Vec3f& operator*=(Vec3f& first, float second);
    Vec3f operator-(const Vec3f& first, const Vec3f& second);
    Vec3f operator-(const Vec3f& first);
    Vec3f operator/(const Vec3f& vec, float scalar);
    Vec3f operator*(const Vec3f& thing, const float& scalar);
    Vec3f operator*(const float& scalar, const Vec3f& thing);
    Vec3f operator+(const Vec3f& first, const Vec3f& second);

    Vec4f operator*(const Mat4& mtx, const Vec4f& vec);
    Vec4f operator/(const Vec4f& thing, const float& scalar);
    Vec4f operator/(const float& scalar, const Vec4f& thing);


    class Rotation
    {
    public:
        float roll, pitch, yaw;

        Rotation() = default;
        Rotation(const Rotation&) = default;
        Rotation(const Vec3f& initRotation) 
            : roll(initRotation.a[0]), pitch(initRotation.a[1]), yaw(initRotation.a[2]) {}
    };

    Vec3f getDirectionFromRotation(const Rotation& rotation);
    float degreesToRadians(float angleInDegrees);
    float radiansToDegrees(float angleInRadians);

/*
    Quaternion conjugate(Quaternion quaternion);
    float moduleSquared(Quaternion quaternion);
    Quaternion inverse(Quaternion quaternion);
    Quaternion operator*(Quaternion quaternion, float scalar);
*/

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