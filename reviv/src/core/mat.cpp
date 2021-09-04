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
    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
            cout << thing.a[i][j] << " ";
        cout << endl;
    }
}

void log(const Mat3& thing)
{
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
            cout << thing.a[i][j] << " ";
        cout << endl;
    }
}

void log(const Vec1f& thing) {
    cout << thing.a[0] << endl;
}

void log(const Vec2f& thing) {
    cout << thing.a[0] << " " << thing.a[1] << endl;
}

void log(const Vec3f& thing)
{
    for(int i=0; i<3; i++)
        cout << thing.a[i] << " ";
    cout << endl;
}

void log(const Vec4f& thing)
{
    for(int i=0; i<4; i++)
        cout << thing.a[i] << " ";
    cout << endl;
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
        : a{x, y} {}

    Vec3f::Vec3f(float x, float y, float z) 
        : a{x, y, z} {}

    Vec4f::Vec4f(float x, float y, float z, float w)
        : a{x, y, z, w} {}

    Vec4f::Vec4f(const Vec3f& vec, float scalar)
        : a {vec.a[0], vec.a[1], vec.a[2], scalar} {}

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
        a[0][0] = a[1][1] = a[2][2] = a[3][3] = n;
    }

    Vec4f operator*(const Mat4& mtx, const Vec4f& vec)
    {
        return multiply(mtx, vec);
    }

    Vec3f operator*(const Vec3f& thing, const float& scalar)
    {
        Vec3f result(thing.a[0] * scalar, thing.a[1] * scalar, thing.a[2] * scalar);
        return result;
    }
    
    Vec3f operator*(const float& scalar, const Vec3f& thing)
    {
        return thing * scalar;
    }

    Vec3f operator+(const Vec3f& first, const Vec3f& second)
    {
        return {first.a[0]+second.a[0], first.a[1]+second.a[1], first.a[2]+second.a[2]};
    }

    Vec4f operator/(const Vec4f& thing, const float& scalar)
    {
        return {thing.a[0]/scalar, thing.a[1]/scalar, thing.a[2]/scalar, thing.a[3]/scalar};
    }

    Vec4f operator/(const float& scalar, const Vec4f& thing)
    {
        return thing / scalar;
    }

    // NOT TESTED
    bool checkIfPointBelongsToLine(const Vec3f& linePoint1, const Vec3f& linePoint2, const Vec3f& point)// TODO: optimize if still used in GJK(https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line)
    {
        float realLenght = module(linePoint1 - linePoint2);
        float calcLength = module(linePoint1 - point) + module(linePoint2 - point); 

        if(abs(realLenght - calcLength) < MAT_EPSILON)
            return true;
        return false;
    }

    // NOT TESTED
    float getDistancePointLine(const Vec3f& point, const Vec3f& lineA, const Vec3f& lineB)
    {
        Vec3f ab = lineB - lineA;
        Vec3f aPoint = point - lineA;

        Vec3f linePerp = cross(cross(ab, aPoint), ab);
        linePerp = linePerp / module(linePerp);

        float perpDistance = dot(linePerp, aPoint);

        Vec3f bPoint = point - lineB;
        if(dot(ab, bPoint) > 0) // on B half space
            return std::min(perpDistance, module(bPoint));

        if(dot(ab, aPoint) < 0)  // on A half space
            return std::min(perpDistance, module(aPoint));

        return perpDistance; // in the "slab" region
    }

/*
    WARNING: not tested
    float calcPointToPlaneDistance(const Vec3f& point, const Vec3f& planePointA, const Vec3f& planePointB, const Vec3f& planePointC) // TODO: can be optimized for GJK, make another function, and take normal and a point(on plane?) as parameters instead of this
    {
        Vec3f abc = cross(planePointB - planePointA, planePointC - planePointA);
        abc = abc / module(abc);
        float result = dot(abc, point);
        result += dot(abc, -planePointA);
    }
*/

    Mat4 translate(Mat4 mtx, const Vec4f& vec)
    {
        mtx.a[0][3] += vec.a[0];
        mtx.a[1][3] += vec.a[1];
        mtx.a[2][3] += vec.a[2];
        mtx.a[3][3] += vec.a[3];
        return mtx;
    }

    Mat4 scale(Mat4 mtx, const Vec4f& vec)
    {            
        for(int i=0; i<4; i++)
            for(int j=0; j<3; j++)
                mtx.a[i][j] *= vec.a[i];

        return mtx;
    }

    Mat4 rotateX(float theta) // supposed to be roll
    {
        Mat4 mtx;

        mtx.a[0][0] = 1;
        mtx.a[0][1] = 0;
        mtx.a[0][2] = 0;
        mtx.a[0][3] = 0;

        mtx.a[1][0] = 0;
        mtx.a[1][1] = cos(theta);
        mtx.a[1][2] = -sin(theta);
        mtx.a[1][3] = 0;

        mtx.a[2][0] = 0;
        mtx.a[2][1] = sin(theta);
        mtx.a[2][2] = cos(theta);
        mtx.a[2][3] = 0;

        mtx.a[3][0] = 0;
        mtx.a[3][1] = 0;
        mtx.a[3][2] = 0;
        mtx.a[3][3] = 1;

        return mtx;
    }

    Mat4 rotateY(float theta) // supposed to be pitch
    {
        Mat4 mtx;
        mtx.a[0][0] = cos(theta);
        mtx.a[0][1] = 0;
        mtx.a[0][2] = -sin(theta);
        mtx.a[0][3] = 0;

        mtx.a[1][0] = 0;
        mtx.a[1][1] = 1;
        mtx.a[1][2] = 0;
        mtx.a[1][3] = 0;

        mtx.a[2][0] = sin(theta);
        mtx.a[2][1] = 0;
        mtx.a[2][2] = cos(theta);
        mtx.a[2][3] = 0;

        mtx.a[3][0] = 0;
        mtx.a[3][1] = 0;
        mtx.a[3][2] = 0;
        mtx.a[3][3] = 1;

        return mtx;
    }

    Mat4 rotateZ(float theta) // supposed to be yaw
    {
        Mat4 mtx;
        mtx.a[0][0] = cos(theta);
        mtx.a[0][1] = -sin(theta);
        mtx.a[0][2] = 0;
        mtx.a[0][3] = 0;

        mtx.a[1][0] = sin(theta);
        mtx.a[1][1] = cos(theta);
        mtx.a[1][2] = 0;
        mtx.a[1][3] = 0;

        mtx.a[2][0] = 0;
        mtx.a[2][1] = 0;
        mtx.a[2][2] = 1;
        mtx.a[2][3] = 0;

        mtx.a[3][0] = 0;
        mtx.a[3][1] = 0;
        mtx.a[3][2] = 0;
        mtx.a[3][3] = 1;

        return mtx;
    }

    Vec4f multiply(const Mat4& mtx, const Vec4f& vec)
    {
        Vec4f rez;
        for(int i=0; i<4; i++)
        {                
            rez.a[i] = 0;
            for(int j=0; j<4; j++)
            {                    
                rez.a[i] += mtx.a[i][j] * vec.a[j];
            }
        }

        return rez;
    }

    Vec3f getDirectionFromRotation(const Rotation& rotation)
    {
        Vec3f direction;
        direction.a[0] = cos(rotation.pitch) * cos(rotation.yaw);
        direction.a[1] = cos(rotation.pitch) * sin(rotation.yaw);
        direction.a[2] = sin(rotation.pitch);

        return direction;
    }

    float module(const Vec4f& vec)
    {
        assert(vec.a[0]*vec.a[0] + vec.a[1]*vec.a[1] + vec.a[2]*vec.a[2] + vec.a[3]*vec.a[3] > 0); // vector maybe of zero length
        return sqrt(vec.a[0]*vec.a[0] + vec.a[1]*vec.a[1] + vec.a[2]*vec.a[2] + vec.a[3]*vec.a[3]);
    }

    float module(const Vec3f& vec)
    {
        assert(vec.a[0]*vec.a[0] + vec.a[1]*vec.a[1] + vec.a[2]*vec.a[2] > 0); // vector maybe of zero length
        return sqrt(vec.a[0]*vec.a[0] + vec.a[1]*vec.a[1] + vec.a[2]*vec.a[2]);
    }

    float module(const Vec2f& vec)
    {
        assert(vec.a[0]*vec.a[0] + vec.a[1]*vec.a[1] > 0); // vector maybe of zero length
        return sqrt(vec.a[0]*vec.a[0] + vec.a[1]*vec.a[1]);
    }

    float module(const Vec1f& vec)
    {
        assert(vec.a[0]*vec.a[0] > 0); // vector maybe of zero length
        return sqrt(vec.a[0]*vec.a[0]);
    }

    float dot(const Vec3f& first, const Vec3f& second)
    {
        return first.a[0]*second.a[0] + first.a[1]*second.a[1] + first.a[2]*second.a[2];
    }

    float dot(const Vec2f& first, const Vec2f& second)
    {
        return first.a[0]*second.a[0] + first.a[1]*second.a[1];
    }

    Vec3f cross(const Vec3f& first, const Vec3f& second)
    {
        Vec3f result;
        result.a[0] = first.a[1] * second.a[2] - first.a[2] * second.a[1];
        result.a[1] = first.a[2] * second.a[0] - first.a[0] * second.a[2];
        result.a[2] = first.a[0] * second.a[1] - first.a[1] * second.a[0];

        return result;
    }

    Vec3f multiply(Vec3f vec, float scalar)
    {
        vec.a[0] *= scalar;
        vec.a[1] *= scalar;
        vec.a[2] *= scalar;
        return vec;
    }

    Vec3f multiply(Vec3f vec, int intiger)
    {
        vec.a[0] *= intiger;
        vec.a[1] *= intiger;
        vec.a[2] *= intiger;
        return vec;
    }

    Vec3f add(const Vec3f& first, const Vec3f& second)
    {
        Vec3f result;
        result.a[0] = first.a[0] + second.a[0];
        result.a[1] = first.a[1] + second.a[1];
        result.a[2] = first.a[2] + second.a[2];
        return result;
    }

    Vec3f subtract(const Vec3f& first, const Vec3f& second)
    {
        return {first.a[0]-second.a[0], first.a[1]-second.a[1], first.a[2]-second.a[2]};
    }

    Vec2f subtract(const Vec2f& first, const Vec2f& second)
    {
        return {first.a[0]-second.a[0], first.a[1]-second.a[1]};
    }

   

    ///
    Vec3f operator-(const Vec3f& first, const Vec3f& second)
    {
        return {first.a[0] - second.a[0], first.a[1] - second.a[1], first.a[2] - second.a[2]};
    }

    Vec3f operator-(const Vec3f& first)
    {
        return {-first.a[0], -first.a[1], -first.a[2]};
    }

    Vec3f& operator+=(Vec3f& first, const Vec3f& second)
    {
        first.a[0] += second.a[0];
        first.a[1] += second.a[1];
        first.a[2] += second.a[2];

        return first;
    }

    Vec3f& operator*=(Vec3f& first, float second)
    {
        first.a[0] *= second;
        first.a[1] *= second;
        first.a[2] *= second;

        return first;
    }

    Vec3f operator/(const Vec3f& first, float scalar)
    {
        return {first.a[0] / scalar, first.a[1] / scalar, first.a[2]/scalar};
    }

    float degreesToRadians(float angleInDegrees)
    {
        return angleInDegrees * 3.14f / 180.f;
    }
    float radiansToDegrees(float angleInRadians)
    {
        return angleInRadians / 3.14f * 180.f;
    }

    Vec2f operator-(const Vec2f& first, const Vec2f& second)
    {
        return subtract(first, second);
    }

    Mat4 multiply(const Mat4& first, const Mat4& second)
    {
        Mat4 result;
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
            {
                result.a[i][j] = 0;
                for(int k=0; k<4; k++)
                    result.a[i][j] += first.a[i][k] * second.a[k][j];
            }

        return result;
    }
};

/*
    Vec3f normalise(Vec3f vec) // NOTE: should be avoided because module of
    {
        float b = module(vec);
        vec.a[0] /= b;
        vec.a[1] /= b;
        vec.a[2] /= b;

        return vec;
    }

    Vec2f normalise(Vec2f vec)
    {
        float b = module(vec);
        vec.x /= b;
        vec.y /= b;
        //vec.a[0] /= b;
        //vec.a[1] /= b;

        return vec;
    }
*/