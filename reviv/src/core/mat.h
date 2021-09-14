#pragma once

#include"rv_pch.hpp"

using std::cin; using std::cout; using std::endl;

namespace mat{

    class Mat1;
    class Mat2;
    class Mat3;
    class Mat4;

    class Vec1f
    {
    public:
        float a[1];
        Vec1f() = default;
        Vec1f(float x);
        Vec1f(const Vec1f&) = default;
        bool operator==(const Vec1f&) = delete;
    };

    class Vec2f
    {
    public:
        float a[2];
        Vec2f() = default;
        Vec2f(float x, float y);
        bool operator==(const Vec2f&) = delete;
        Vec2f(const Vec2f&) = default;
    };

    class Vec3f
    {
    public:
        Vec3f() = default;
        Vec3f(float x, float y, float z);
        Vec3f(const Vec3f&) = default;
        bool operator==(const Vec3f&) = delete;
        float a[3];
    };

    class Vec4f
    {
    public:
        float a[4];
        Vec4f() = default;
        Vec4f(float x, float y, float z, float w);
        Vec4f(const Vec4f&) = default;
        bool operator==(const Vec4f&) = delete;
        Vec4f(const Vec3f& vec, float scalar);
    };

    class MatN // NOTE: dynamically alocated
    {
    public:
        MatN(int height, int width);
        ~MatN();

        MatN(const MatN&) = delete;
        MatN& operator=(const MatN& other) = delete;
        MatN& operator==(const MatN& other) = delete;

        inline float get(int indexHeight, int indexWidth) const
        {
            return *(pData + TwoDimIndexToLinearIndex(indexHeight, indexWidth));
        }
        inline float* getPtr(int indexHeight, int indexWidth)
        {
            return (pData + TwoDimIndexToLinearIndex(indexHeight, indexWidth));
        }

        int height = 0;
        int width = 0;

    private:
        inline unsigned int TwoDimIndexToLinearIndex(int indexHeight, int indexWidth) const
        {
            assert(indexHeight < height);
            assert(indexWidth < width);
            assert(indexHeight >= 0);
            assert(indexHeight >= 0);
            assert(indexWidth >= 0);
            assert(pData != nullptr);

            return indexHeight * width + indexWidth;
        };
        float* pData = nullptr;
    };

    void multiply(MatN* pResult, const MatN& first, const MatN& second);
    void transpose(MatN* pResult, const MatN& mtx);

    class Mat1
    {
    public:
        float a[1][1];
        Mat1() = default;
        Mat1(const Mat1&) = default;
        bool operator==(const Mat1&) = delete;
    };

    class Mat2
    {
    public:
        float a[2][2];
        Mat2() = default;
        Mat2(const Mat2&) = default;
        bool operator==(const Mat2&) = delete;
    };

    class Mat3
    {
    public:
        float a[3][3];
        Mat3() = default;
        Mat3(float n);
        Mat3(const Mat3&) = default;
        Mat3(const Mat4& upperLeft); // copies the upper left part of the Mat4 into the Mat3
        bool operator==(const Mat3&) = delete;
    };

    class Mat4
    {
    public:
        float a[4][4];
        Mat4() = default;
        Mat4(float n);
        Mat4(const Mat4&) = default;
        bool operator==(const Mat4&) = delete;
    };
    
    struct Quaternion
    {
        float x[4];
    };

    bool compare(const Vec3f& first, const Vec3f& second, float marginOfError);

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
    Mat4 scale(const Vec4f& vec);
    Mat4 rotateX(float theta); // supposed to be roll
    Mat4 rotateY(float theta); // supposed to be pitch
    Mat4 rotateZ(float theta); // supposed to be yaw

    float getDeterminant(const Mat3& mtx);

    Mat1 transpose(const Mat1& mtx);
    // not tested
    Mat3 transpose(const Mat3& mtx);

    Mat1 inverse(const Mat1& mtx);
    Mat3 inverse(const Mat3& mtx);

    Mat4 multiply(const Mat4& first, const Mat4& second); // TODO: maybe canbe templated (so the user just does multiply(thing, thing2) without knowing that it is even templated)
    Mat3 multiply(const Mat3& first, const Mat3& second);
    Vec4f multiply(const Mat4& mtx, const Vec4f& vec);
    Vec3f multiplyScalar(Vec3f vec, float scalar);
    Vec3f multiplyScalar(Vec3f vec, int intiger);

    float module(const Vec4f& vec);
    float module(const Vec3f& vec);
    float module(const Vec2f& vec);
    float module(const Vec1f& vec);

    Vec3f cross(const Vec3f& first, const Vec3f& second);
    float dot(const Vec3f& first, const Vec3f& second);
    float dot(const Vec2f& first, const Vec2f& second);
    Vec3f projection(const Vec3f& first, const Vec3f& second);

    Vec3f add(const Vec3f& a, const Vec3f& b);
    Vec2f subtract(const Vec2f& first, const Vec2f& second);
    Vec3f subtract(const Vec3f& first, const Vec3f& second);

    Mat4 operator*(const Mat4& first, const Mat4& second);
    Mat3 operator*(const Mat3& first, const Mat3& second);
    Vec3f operator*(const Vec3f& thing, const float& scalar);
    Vec3f operator*(const float& scalar, const Vec3f& thing);
    Vec4f operator*(const Mat4& mtx, const Vec4f& vec);
    Vec3f operator*(const Mat3& mtx, const Vec3f& vec);

    Vec4f operator/(const Vec4f& thing, const float& scalar);
    Vec4f operator/(const float& scalar, const Vec4f& thing);
    Vec3f operator/(const Vec3f& vec, float scalar);

    Vec3f operator+(const Vec3f& first, const Vec3f& second);
    Vec3f operator-(const Vec3f& first, const Vec3f& second);
    Vec3f operator-(const Vec3f& first);
    Vec2f operator-(const Vec2f& first, const Vec2f& second);

    Vec3f& operator+=(Vec3f& first, const Vec3f& second);
    Vec3f& operator*=(Vec3f& first, float second);



    class Rotation
    {
    public:
        float roll, pitch, yaw;

        Rotation() = default;
        Rotation(const Rotation&) = default;
        Rotation(const Vec3f& initRotation) 
            : roll(initRotation.a[0]), pitch(initRotation.a[1]), yaw(initRotation.a[2]) {}
    };
    Rotation lookAtGetRotation(const Vec3f& eyePosition, const Vec3f& targetPosition);
    Vec3f getDirectionFromRotation(const Rotation& rotation);
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

void log(const MatN& thing);

void log(const Mat3& thing);
void log(const Mat4& thing);

void log(const bool& thing);
void log(const int thing);
void log(const Rotation& thing);