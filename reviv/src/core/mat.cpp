#include "mat.h"

const Mat4 identity(1);

#define MAT_EPSILON 0.00001f

void log(const bool& thing)
{
    assert(false); // Error: using this overloaded function log means that something went wrong

    if(thing == true)
    {
        cout << "true";
    }
    else
    {
        cout << "false";
    }
    cout << endl;
}


void log(const Rotation& thing)
{
    cout << "pitch=" << thing.pitch << " yaw=" << thing.yaw << " roll=" << thing.roll << endl;
}

void log(const Mat4& thing)
{
    cout << thing.a.x << " " << thing.b.x << " " << thing.c.x << " " << thing.d.x << endl;
    cout << thing.a.y << " " << thing.b.y << " " << thing.c.y << " " << thing.d.y << endl;
    cout << thing.a.z << " " << thing.b.z << " " << thing.c.z << " " << thing.d.z << endl;
    cout << thing.a.w << " " << thing.b.w << " " << thing.c.w << " " << thing.d.w << endl;
}

void log(const Mat3& thing)
{
    cout << thing.a.x << " " << thing.b.x << " " << thing.c.x << endl;
    cout << thing.a.y << " " << thing.b.y << " " << thing.c.y << endl;
    cout << thing.a.z << " " << thing.b.z << " " << thing.c.z << endl;
}

void log(const Vec1f& thing) {
    cout << thing.x << endl;
}

void log(const Vec2f& thing) {
    cout << thing.x << " " << thing.y << endl;
}

void log(const Vec3f& thing)
{
    cout << thing.x << " " << thing.y << " " << thing.z << endl;
}

void log(const Vec4f& thing)
{
    cout << thing.x << " " << thing.y << " " << thing.z << " " << thing.w << endl;
}

void log(const Vec1i& thing)
{
    cout << thing.x << endl;
}

void log(const Vec2i& thing)
{
    cout << thing.x << " " << thing.y << endl;
}

void log(const Vec3i& thing)
{
    cout << thing.x << " " << thing.y << " " << thing.z << endl;
}

void log(const Vec4i& thing)
{
    cout << thing.x << " " << thing.y << " " << thing.z << " " << thing.w << endl;
}


namespace mat{

    Vec2f::Vec2f(float x, float y) 
        : x(x), y(y) {}

    Vec3f::Vec3f(float x, float y, float z) 
        : x(x), y(y), z(z) {}

    Vec4f::Vec4f(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {}

    Vec1i::Vec1i(int x)
        : x(x) {}

    Vec2i::Vec2i(int x, int y)
        : x(x), y(y) {}

    Vec3i::Vec3i(int x, int y, int z)
        : x(x), y(y), z(z) {}

    Vec4i::Vec4i(int x, int y, int z, int w)
        : x(x), y(y), z(z), w(w) {}

    Mat4::Mat4(float n)
    {
        a.x = b.y = c.z = d.w = n;
    }

    Vec3f operator*(const Vec3f& thing, const float& scalar)
    {
        Vec3f result(thing.x * scalar, thing.y * scalar, thing.z * scalar);
        return result;
    }
    
    Vec3f operator*(const float& scalar, const Vec3f& thing)
    {
        return thing * scalar;
    }

    Vec4f operator/(const Vec4f& thing, const float& scalar)
    {
        return {thing.x/scalar, thing.y/scalar, thing.z/scalar, thing.w/scalar};
    }

    Vec4f operator/(const float& scalar, const Vec4f& thing)
    {
        return thing / scalar;
    }

    Mat4 translate(Mat4 a, const Vec3f& b)
    {
        a.d.x += b.x;
        a.d.y += b.y;
        a.d.z += b.z;
        return a;
    }

    Mat4 scale(Mat4 a, const Vec3f& b)
    {
        a.a.x *= b.x;
        a.b.y *= b.y;
        a.c.z *= b.z;
        return a;
    }

    Mat4 rotateX(Mat4 a, float theta)
    {
        a.a.x = 1;
        a.b.y = cos(theta);
        a.b.z = sin(theta);
        a.c.y = -sin(theta);
        a.c.z = cos(theta);
        a.d.w = 1;
        return a;
    }

    Mat4 rotateY(Mat4 a, float theta)
    {
        a.a.x = cos(theta);
        a.a.z = -sin(theta);
        a.b.y = 1;
        a.c.x = sin(theta);
        a.c.z = cos(theta);
        a.d.w = 1;
        return a;
    }

    Mat4 rotateZ(Mat4 a, float theta)
    {
        a.a.x = cos(theta);
        a.a.y = sin(theta);
        a.b.x = -sin(theta);
        a.b.y = cos(theta);
        a.c.z = 1;
        a.c.w = 1;
        return a;
    }

    Vec4f multiply(const Mat4& matrix, const Vec4f& vec)
    {
        //std::cout << "BBB";
        Vec4f rez;
        rez.x = matrix.a.x * vec.x + matrix.b.x * vec.y + matrix.c.x * vec.z + matrix.d.x * vec.w;
        rez.y = matrix.a.y * vec.x + matrix.b.y * vec.y + matrix.c.y * vec.z + matrix.d.y * vec.w;
        rez.z = matrix.a.z * vec.x + matrix.b.z * vec.y + matrix.c.z * vec.z + matrix.d.z * vec.w;
        rez.w = matrix.a.w * vec.x + matrix.b.w * vec.y + matrix.c.w * vec.z + matrix.d.w * vec.w;

        return rez;
    }

    Vec3f getDirectionFromRotation(const Rotation& rotation)
    {
        Vec3f direction;
        direction.x = cos(rotation.pitch) * cos(rotation.yaw);
        direction.y = cos(rotation.pitch) * sin(rotation.yaw);
        direction.z = sin(rotation.pitch);

        return direction;
    }

    float module(const Vec3f& a)
    {
        return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + MAT_EPSILON);
    }

    float module(const Vec2f& a)
    {
        return sqrt(a.x * a.x + a.y * a.y + MAT_EPSILON);
    }

    float dot(const Vec3f& a, const Vec3f& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    float dot(const Vec2f& a, const Vec2f& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    Vec3f projection(const Vec3f& a, const Vec3f& b)
    {
        float d = (module(b)) * (module(b)) / dot(a, b);
        Vec3f c;
        c.x = a.x * d;
        c.y = a.y * d;
        c.z = a.z * d;
        return c;
    }

    Vec3f normalise(Vec3f a)
    {
        float b = module(a);
        a.x /= b;
        a.y /= b;
        a.z /= b;

        return a;
    }

    Vec2f normalise(Vec2f a)
    {
        float b = module(a);
        a.x /= b;
        a.y /= b;

        return a;
    }

    Vec3f cross(const Vec3f& a, const Vec3f& b)
    {
        Vec3f rez;
        rez.x = a.y * b.z - a.z * b.y;
        rez.y = a.z * b.x - a.x * b.z;
        rez.z = a.x * b.y - a.y * b.x;
        return rez;
    }

    Vec3f multiplyScalar(Vec3f a, float b)
    {
        a.x *= b;
        a.y *= b;
        a.z *= b;
        return a;
    }

    Vec3f multiplyScalar(Vec3f a, int b)
    {
        a.x *= b;
        a.y *= b;
        a.z *= b;
        return a;
    }

    Vec3f add(const Vec3f& a, const Vec3f& b)
    {
        Vec3f c;
        c.x = a.x + b.x;
        c.y = a.y + b.y;
        c.z = a.z + b.z;
        return c;
    }

    Vec3f subtract(const Vec3f& a, const Vec3f& b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    Vec2f subtract(const Vec2f& a, const Vec2f& b)
    {
        return {a.x - b.x, a.y - b.y};
    }

    int sgn(int n)
    {
        if (n > 0)
            return 1;
        else return -1;
    }

    int sgn(float n)
    {
        if (n > 0)
            return 1;
        else return -1;
    }

    float clampTwoSide(float minPossibleValue, float maxPossibleValue, float n)
    {
        if (n < minPossibleValue)
            return minPossibleValue;
        if (n > maxPossibleValue)
            return maxPossibleValue;
        return n;

    }

    float clampMin(float minPossibleValue, float n)
    {
        if (n < minPossibleValue)
            return minPossibleValue;
        else return n;
    }

    float clampMax(float maxPossibleValue, float n)
    {
        if (n > maxPossibleValue)
            return maxPossibleValue;
        else return n;
    }

    float degreesToRadians(float angleInDegrees)
    {
        return angleInDegrees * 3.14f / 180.f;
    }
    float radiansToDegrees(float angleInRadians)
    {
        return angleInRadians / 3.14f * 180.f;
    }

    Mat4 multiply(const Mat4& first, const Mat4& second)
    {
        Mat4 result;
        result.a.x =    first.a.x * second.a.x 
                       +first.b.x * second.a.y
                       +first.c.x * second.a.z
                       +first.d.x * second.a.w;

        result.b.x =    first.a.x * second.b.x 
                       +first.b.x * second.b.y
                       +first.c.x * second.b.z
                       +first.d.x * second.b.w;

        result.c.x =    first.a.x * second.c.x 
                       +first.b.x * second.c.y
                       +first.c.x * second.c.z
                       +first.d.x * second.c.w;

        result.d.x =    first.a.x * second.d.x 
                       +first.b.x * second.d.y
                       +first.c.x * second.d.z
                       +first.d.x * second.d.w;



        result.a.y =    first.a.y * second.a.x 
                       +first.b.y * second.a.y
                       +first.c.y * second.a.z
                       +first.d.y * second.a.w;

        result.b.y =    first.a.y * second.b.x 
                       +first.b.y * second.b.y
                       +first.c.y * second.b.z
                       +first.d.y * second.b.w;

        result.c.y =    first.a.y * second.c.x 
                       +first.b.y * second.c.y
                       +first.c.y * second.c.z
                       +first.d.y * second.c.w;

        result.d.y =    first.a.y * second.d.x 
                       +first.b.y * second.d.y
                       +first.c.y * second.d.z
                       +first.d.y * second.d.w;




        result.a.z =    first.a.z * second.a.x 
                       +first.b.z * second.a.y
                       +first.c.z * second.a.z
                       +first.d.z * second.a.w;

        result.b.z =    first.a.z * second.b.x 
                       +first.b.z * second.b.y
                       +first.c.z * second.b.z
                       +first.d.z * second.b.w;

        result.c.z =    first.a.z * second.c.x 
                       +first.b.z * second.c.y
                       +first.c.z * second.c.z
                       +first.d.z * second.c.w;

        result.d.z =    first.a.z * second.d.x 
                       +first.b.z * second.d.y
                       +first.c.z * second.d.z
                       +first.d.z * second.d.w;



        result.a.w =    first.a.w * second.a.x 
                       +first.b.w * second.a.y
                       +first.c.w * second.a.z
                       +first.d.w * second.a.w;

        result.b.w =    first.a.w * second.b.x 
                       +first.b.w * second.b.y
                       +first.c.w * second.b.z
                       +first.d.w * second.b.w;

        result.c.w =    first.a.w * second.c.x 
                       +first.b.w * second.c.y
                       +first.c.w * second.c.z
                       +first.d.w * second.c.w;

        result.d.w =    first.a.w * second.d.x 
                       +first.b.w * second.d.y
                       +first.c.w * second.d.z
                       +first.d.w * second.d.w;

        return result;
    }
};
