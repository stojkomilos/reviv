#pragma once

#include<iostream>
#include<cmath>

// NOVO TODO: stavi da log metoda bude van strukture, mozda cak i van namespace-a
// NOVO TODO: optimizovanje koriscenjem "initializer list" i copy construtor. takodje, const refererence za mat::add, itd
// NOVO TODO: operatori da se koriste: add, multiply, ==, etc...
// NOVO TODO: operator >> da se koristi za cout
// NOVO TODO: kapitalizacija SVIH klasa i struktura ovde i pretvori ih odma sve i u klase

using std::cin; using std::cout; using std::endl;

namespace mat{

    class Vec1f{
    public:
        float x;
        Vec1f();
        Vec1f(float x);
    };
    class Vec2f{
    public:
        float x, y;
        Vec2f();
        Vec2f(float x, float y);
    };
    class Vec3f{
    public:
        float x, y, z;
        Vec3f();
        Vec3f(float x, float y, float z);
    };
    class Vec4f{
    public:
        float x, y, z, w;
        Vec4f();
        Vec4f(float x, float y, float z, float w);
    };
    class Mat4{
    public:
        Vec4f a, b, c, d;
        Mat4();
        Mat4(float n);
    };

    Mat4 translate(Mat4 a, const Vec3f& b);
    Mat4 scale(Mat4 a, const Vec3f& b);
    Mat4 rotateX(Mat4 a, float theta);
    Mat4 rotateY(Mat4 a, float theta);
    Mat4 rotateZ(Mat4 a, float theta);

    Mat4 perspective(float close, float far, float fov, float R);

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
    int sgn(int n);
    int sgn(float n);
    float clampTwoSide(float minPossibleValue, float maxPossibleValue, float n);
    float clampMin(float minPossibleValue, float n);
    float clampMax(float maxPossibleValue, float n);

    //class Camera{
    //public:
    //    Vec3f position, right, up, direction;
    //    Mat4 viewMatrix;
    //    float pitch, yaw;
    //    void setViewMatrix();
    //    void setViewMatrix(const Vec3f& position, const Vec3f& direction);
    //    void setViewMatrix(const Vec3f& position, float pitch1, float yaw1);
    //    Camera();

    //};

    class Rotation
    {
    public:
        float pitch, yaw, roll;

        Rotation() = default;
        Rotation(const Rotation&) = default;
        Rotation(const Vec3f& initRotation) 
            : pitch(initRotation.x), yaw(initRotation.y), roll(initRotation.z) {}

    //        operator const Vec3f& () const { return Vec3f(pitch, yaw, roll); }
    //        operator Vec3f& () { return Vec3f(pitch, yaw, roll); }
    };
}

using namespace mat;

void log(const Vec3f& thing);
void log(const Mat4& thing);
void log(const Vec2f& thing);
void log(const Rotation& thing);

extern Mat4 identity;