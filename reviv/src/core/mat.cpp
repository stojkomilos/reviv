#include "mat.h"

#define MAT_EPSILON 0.00001f


namespace mat{

    float random(float lowerLimit, float upperLimit)
    {
        return (rand() / ((RAND_MAX-1) * 1.f)) * (upperLimit - lowerLimit) + lowerLimit;
    };

    // NOT TESTED
    bool checkIfPointBelongsToLine(const Vec3& linePoint1, const Vec3& linePoint2, const Vec3& point)// TODO: optimize if still used in GJK(https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line)
    {
        float realLenght = module(linePoint1 - linePoint2);
        float calcLength = module(linePoint1 - point) + module(linePoint2 - point); 

        if(abs(realLenght - calcLength) < MAT_EPSILON)
            return true;
        return false;
    }

    // NOT TESTED
    float getDistancePointLine(const Vec3& point, const Vec3& lineA, const Vec3& lineB)
    {
        Vec3 ab = lineB - lineA;
        Vec3 aPoint = point - lineA;

        Vec3 linePerp = cross(cross(ab, aPoint), ab);
        linePerp = linePerp / module(linePerp);

        float perpDistance = dot(linePerp, aPoint);

        Vec3 bPoint = point - lineB;
        if(dot(ab, bPoint) > 0) // on B half space
            return std::min(perpDistance, module(bPoint));

        if(dot(ab, aPoint) < 0)  // on A half space
            return std::min(perpDistance, module(aPoint));

        return perpDistance; // in the "slab" region
    }

/*
    WARNING: not tested
    float calcPointToPlaneDistance(const Vec3& point, const Vec3& planePointA, const Vec3& planePointB, const Vec3& planePointC) // TODO: can be optimized for GJK, make another function, and take normal and a point(on plane?) as parameters instead of this
    {
        Vec3 abc = cross(planePointB - planePointA, planePointC - planePointA);
        abc = abc / module(abc);
        float result = dot(abc, point);
        result += dot(abc, -planePointA);
    }
*/

    Vec3 cross(const Vec3& first, const Vec3& second)
    {
        Vec3 result;
        *result.getPtr(0, 0) = first.get(1, 0) * second.get(2, 0) - first.get(2, 0) * second.get(1, 0);
        *result.getPtr(1, 0) = -first.get(0, 0) * second.get(2, 0) + first.get(2, 0) * second.get(0, 0);
        *result.getPtr(2, 0) = first.get(0, 0) * second.get(1, 0) - first.get(1, 0) * second.get(0, 0);

        return result;
    }

    Mat<4, 4> translate(Mat<4, 4> mtx, const Vec4& vec)
    {
        *mtx.getPtr(0, 3) += vec.get(0, 0);
        *mtx.getPtr(1, 3) += vec.get(1, 0);
        *mtx.getPtr(2, 3) += vec.get(2, 0);
        *mtx.getPtr(3, 3) += vec.get(3, 0);

        return mtx;
    }


    Mat<4, 4> rotateX(float theta) // supposed to be get(0, 0)
    {
        Mat<4, 4> mtx;

        *mtx.getPtr(0, 0) = 1;
        *mtx.getPtr(0, 1) = 0;
        *mtx.getPtr(0, 2) = 0;
        *mtx.getPtr(0, 3) = 0;

        *mtx.getPtr(1, 0) = 0;
        *mtx.getPtr(1, 1) = cos(theta);
        *mtx.getPtr(1, 2) = -sin(theta);
        *mtx.getPtr(1, 3) = 0;

        *mtx.getPtr(2, 0) = 0;
        *mtx.getPtr(2, 1) = sin(theta);
        *mtx.getPtr(2, 2) = cos(theta);
        *mtx.getPtr(2, 3) = 0;

        *mtx.getPtr(3, 0) = 0;
        *mtx.getPtr(3, 1) = 0;
        *mtx.getPtr(3, 2) = 0;
        *mtx.getPtr(3, 3) = 1;

        return mtx;
    }

    Mat<4, 4> rotateY(float theta) // supposed to be get(1, 0)
    {
        Mat<4, 4> mtx;
        *mtx.getPtr(0, 0) = cos(theta);
        *mtx.getPtr(0, 1) = 0;
        *mtx.getPtr(0, 2) = -sin(theta);
        *mtx.getPtr(0, 3) = 0;

        *mtx.getPtr(1, 0) = 0;
        *mtx.getPtr(1, 1) = 1;
        *mtx.getPtr(1, 2) = 0;
        *mtx.getPtr(1, 3) = 0;

        *mtx.getPtr(2, 0) = sin(theta);
        *mtx.getPtr(2, 1) = 0;
        *mtx.getPtr(2, 2) = cos(theta);
        *mtx.getPtr(2, 3) = 0;

        *mtx.getPtr(3, 0) = 0;
        *mtx.getPtr(3, 1) = 0;
        *mtx.getPtr(3, 2) = 0;
        *mtx.getPtr(3, 3) = 1;

        return mtx;
    }

    Mat<4, 4> rotateZ(float theta) // supposed to be get(2, 0)
    {
        Mat<4, 4> mtx;
        *mtx.getPtr(0, 0) = cos(theta);
        *mtx.getPtr(0, 1) = -sin(theta);
        *mtx.getPtr(0, 2) = 0;
        *mtx.getPtr(0, 3) = 0;

        *mtx.getPtr(1, 0) = sin(theta);
        *mtx.getPtr(1, 1) = cos(theta);
        *mtx.getPtr(1, 2) = 0;
        *mtx.getPtr(1, 3) = 0;

        *mtx.getPtr(2, 0) = 0;
        *mtx.getPtr(2, 1) = 0;
        *mtx.getPtr(2, 2) = 1;
        *mtx.getPtr(2, 3) = 0;

        *mtx.getPtr(3, 0) = 0;
        *mtx.getPtr(3, 1) = 0;
        *mtx.getPtr(3, 2) = 0;
        *mtx.getPtr(3, 3) = 1;

        return mtx;
    }


    Vec3 getDirectionFromRotation(const Vec3& rotation)
    {
        Vec3 direction;
        *direction.getPtr(0, 0) = cos(rotation.get(1, 0)) * cos(rotation.get(2, 0));
        *direction.getPtr(1, 0) = cos(rotation.get(1, 0)) * sin(rotation.get(2, 0));
        *direction.getPtr(2, 0) = sin(rotation.get(1, 0));

        return direction;
    }

    float degreesToRadians(float angleInDegrees)
    {
        return angleInDegrees * 3.14f / 180.f;
    }
    float radiansToDegrees(float angleInRadians)
    {
        return angleInRadians / 3.14f * 180.f;
    }

    Vec3 lookAtGetRotation(const Vec3& eyePosition, const Vec3& targetPosition)
    {
        Vec3 rotation;
        float distance = module(eyePosition - targetPosition);

        *rotation.getPtr(1, 0) = asin((targetPosition.get(2, 0) - eyePosition.get(2, 0)) / distance);
        *rotation.getPtr(0, 0) = 0;
        *rotation.getPtr(2, 0) = atan2(targetPosition.get(1, 0) - eyePosition.get(1, 0), targetPosition.get(0, 0) - eyePosition.get(0, 0));

        return rotation;
    }

    float getDeterminant(const Mat<3,3>& mtx)
    {
        return mtx.get(0, 0) * (mtx.get(1, 1) * mtx.get(2, 2) - mtx.get(2, 1) * mtx.get(1, 2))
            -  mtx.get(0, 1) * (mtx.get(1, 0) * mtx.get(2, 2) - mtx.get(2, 0) * mtx.get(1, 2))
            +  mtx.get(0, 2) * (mtx.get(1, 0) * mtx.get(2, 1) - mtx.get(2, 0) * mtx.get(1, 1));
    }

    Mat<3,3> inverse(const Mat<3,3>& mtx)
    {
        float invDet = 1 / getDeterminant(mtx);
        Mat<3,3> result;

        *result.getPtr(0, 0) = +(mtx.get(1, 1) * mtx.get(2, 2) - mtx.get(2, 1) * mtx.get(1, 2)) * invDet;
        *result.getPtr(1, 0) = -(mtx.get(1, 0) * mtx.get(2, 2) - mtx.get(2, 0) * mtx.get(1, 2)) * invDet;
        *result.getPtr(2, 0) = +(mtx.get(1, 0) * mtx.get(2, 1) - mtx.get(2, 0) * mtx.get(1, 1)) * invDet;

        *result.getPtr(0, 1) = -(mtx.get(0, 1) * mtx.get(2, 2) - mtx.get(2, 1) * mtx.get(0, 2)) * invDet;
        *result.getPtr(1, 1) = +(mtx.get(0, 0) * mtx.get(2, 2) - mtx.get(2, 0) * mtx.get(0, 2)) * invDet;
        *result.getPtr(2, 1) = -(mtx.get(0, 0) * mtx.get(2, 1) - mtx.get(2, 0) * mtx.get(0, 1)) * invDet;

        *result.getPtr(0, 2) = +(mtx.get(0, 1) * mtx.get(1, 2) - mtx.get(1, 1) * mtx.get(0, 2)) * invDet;
        *result.getPtr(1, 2) = -(mtx.get(0, 0) * mtx.get(1, 2) - mtx.get(1, 0) * mtx.get(0, 2)) * invDet;
        *result.getPtr(2, 2) = +(mtx.get(0, 0) * mtx.get(1, 1) - mtx.get(1, 0) * mtx.get(0, 1)) * invDet;

        return result;
    }


    //// MapHeap stuf

    MatHeap::MatHeap(int height, int width)
        : height(height), width(width)
    {
        assert(pData == nullptr);
        assert(height >= 0 && width >= 0);

        pData = new float[height * width];
    }

    MatHeap::MatHeap(int height, int width, float fillInValue)
        : MatHeap(height, width)
    {
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
                *getPtr(i, j) = fillInValue;
    }

    MatHeap::~MatHeap()
    {
        delete[] pData;
    }

    void multiply(MatHeap* pResult, const MatHeap& first, const MatHeap& second)
    {
        assert(first.width == second.height);
        assert(pResult->height == first.height && pResult->width == second.width);

        for(int i=0; i<first.height; i++)
            for(int j=0; j<second.width; j++)
            {
                *pResult->getPtr(i, j) = 0;
                for(int k=0; k<first.width; k++)
                    *pResult->getPtr(i, j) += first.get(i, k) * second.get(k, j);
            }
    }

    void multiply(MatHeap* pResult, const MatHeap& mtx, float scalar)
    {
        assert(pResult->height == mtx.height && pResult->width == mtx.width);

        for(int i=0; i<mtx.height; i++)
            for(int j=0; j<mtx.width; j++)
                *pResult->getPtr(i, j) = mtx.get(i, j) * scalar;
    }

    void subtract(MatHeap* pResult, const MatHeap& first, const MatHeap& second)
    {
        assert(pResult->height == first.height && pResult->width == first.width);
        assert(second.height == first.height && second.width == first.width);

        for(int i=0; i<first.height; i++)
            for(int j=0; j<first.width; j++)
                *pResult->getPtr(i, j) = first.get(i, j) - second.get(i, j);
    }

    void MatHeap::setToIdentity()
    {
        assert(width == height); // only square matrices can be identity matrices
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
                if(i != j)
                    *getPtr(i, j) = 0;
                else *getPtr(i, i) = 1;
    }

    void transpose(MatHeap* pResult, const MatHeap& mtx)
    {
        assert(pResult->height == mtx.width && pResult->width == mtx.height);
        assert(pResult != &mtx);

        for(int i=0; i<pResult->height; i++)
            for(int j=0; j<pResult->width; j++)
                *pResult->getPtr(i, j) = mtx.get(j, i);
    }

    void inverse(MatHeap* pResult, const MatHeap& mtx)
    {
        assert(false);
    }

    float module(const MatHeap& mtx)
    {
        assert(mtx.width == 1);
        float result = 0.f;

        for(int i=0; i<mtx.height; i++)
            result += mtx.get(i, 0) * mtx.get(i, 0);

        return sqrt(result);
    }

    void MatHeap::setTo(const MatHeap& other)
    {
        assert(height == other.height && width == other.width);
        for(int i=0; i<height; i++)
            for(int j=0; j<width; j++)
                *getPtr(i, j) = other.get(i, j);
    }

    MatHeap::MatHeap(const MatHeap& other)
        : MatHeap(other.height, other.width)
    {
        setTo(other);
    }

    void add(MatHeap* pResult, const MatHeap& first, const MatHeap& second)
    {
        assert(pResult->height == first.height && pResult->width == first.width);
        assert(second.height == first.height && second.width == first.width);

        for(int i=0; i<first.height; i++)
            for(int j=0; j<first.width; j++)
                *pResult->getPtr(i, j) = first.get(i, j) + second.get(i, j);
    }
 
};

void log(const MatHeap& thing)
{
    for(int i=0; i<thing.height; i++)
    {
        for(int j=0; j<thing.width; j++)
            cout << thing.get(i, j) << " ";
        cout << endl;
    }
}