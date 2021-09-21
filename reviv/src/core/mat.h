#pragma once

#include"rv_pch.hpp"

using std::cin; using std::cout; using std::endl;

namespace mat{

    float random(float lowerLimit, float upperLimit);

    template<int HEIGHT, int WIDTH>
    class Mat
    {
    public:
        Mat();

        Mat(float x);
        Mat(float x, float y);
        Mat(float x, float y, float z);
        Mat(float x, float y, float z, float w);
        Mat(const Mat<HEIGHT-1, 1>& other, float scalar);

        Mat(const Mat<HEIGHT, WIDTH>& other);
        const Mat<HEIGHT, WIDTH>& operator=(const Mat<HEIGHT, WIDTH>& other);

        void setTo(const Mat<HEIGHT, WIDTH>& other);
        void randomize(float lowerLimit, float upperLimit);
        void setToIdentity();
        void fill(float n);

        void add(const Mat<HEIGHT, WIDTH>& other);
        void subtract(const Mat<HEIGHT, WIDTH>& other);
        void multiply(float scalar);

        template <size_t OTHER_WIDTH>
        void multiply(const Mat<HEIGHT, OTHER_WIDTH>& other);

        constexpr size_t getHeight() { return HEIGHT; } // needs constexpr getter for templates
        constexpr size_t getWidth() { return WIDTH; }

        inline float get(int column, int row) const
        {
            assert(column >= 0);
            assert(row >= 0);
            assert(column < HEIGHT);
            assert(row < WIDTH);
            return data[column][row];
        }
        inline float* getPtr(int column, int row)
        {
            assert(column >= 0);
            assert(row >= 0);
            assert(column < HEIGHT);
            assert(row < WIDTH);
            return &data[column][row];
        }

        inline const float* getPtr(int column, int row) const
        {
            assert(column >= 0);
            assert(row >= 0);
            assert(column < HEIGHT);
            assert(row < WIDTH);
            return &data[column][row];
        }

        static const size_t height = HEIGHT;
        static const size_t width = WIDTH;
    private:
        float data[HEIGHT][WIDTH];
    };

    using Vec1 = Mat<1, 1>;
    using Vec2 = Mat<2, 1>;
    using Vec3 = Mat<3, 1>;
    using Vec4 = Mat<4, 1>;

    Vec3 lookAtGetRotation(const Vec3& eyePosition, const Vec3& targetPosition);
    Vec3 getDirectionFromRotation(const Vec3& rotation);
    Vec3 getDirectionFromRotation(const Vec3& rotation);

    float degreesToRadians(float angleInDegrees);
    float radiansToDegrees(float angleInRadians);

    Mat<4, 4> rotateX(float theta); // supposed to be roll
    Mat<4, 4> rotateY(float theta); // supposed to be pitch
    Mat<4, 4> rotateZ(float theta); // supposed to be yaw

    Mat<3,3> inverse(const Mat<3,3>& mtx);
    float getDeterminant(const Mat<3,3>& mtx);
    Vec3 cross(const Vec3& first, const Vec3& second);

    // NOT TESTED
    bool checkIfPointBelongsToLine(const Vec3& linePoint1, const Vec3& linePoint2, const Vec3& point);

    // NOT TESTED
    float getDistancePointLine(const Vec3& point, const Vec3& lineA, const Vec3& lineB);

    inline bool sign(float a)
    { 
        if(a>0) 
            return true; 
        return false; 
    }

    Mat<4, 4> translate(Mat<4, 4> mtx, const Vec4& vec);


    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>::Mat(float x)
        : Mat()
    {
        assert(HEIGHT == 1 && WIDTH == 1); // Mat is not a 1-dimensional vector
        data[0][0] = x;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>::Mat(float x, float y)
        : Mat()
    {
        assert(HEIGHT == 2 && WIDTH == 1); // Mat is not a 1-dimensional vector
        data[0][0] = x;
        data[1][0] = y;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>::Mat(float x, float y, float z)
        : Mat()
    {
        assert(HEIGHT == 3 && WIDTH == 1); // Mat is not a 1-dimensional vector
        data[0][0] = x;
        data[1][0] = y;
        data[2][0] = z;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>::Mat(float x, float y, float z, float w)
        : Mat()
    {
        assert(HEIGHT == 4 && WIDTH == 1); // Mat is not a 1-dimensional vector
        data[0][0] = x;
        data[1][0] = y;
        data[2][0] = z;
        data[3][0] = z;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>::Mat(const Mat<HEIGHT-1, 1>& other, float scalar)
        : Mat()
    {
        assert(WIDTH == 1 && other.width == 1); // this sort of constructor can only be used with vectors

        for(int i=0; i<HEIGHT-1; i++)
            data[i][0] = other.get(i, 0);

        data[HEIGHT-1][0] = scalar;
    }

    template<int HEIGHT, int WIDTH>
    void Mat<HEIGHT, WIDTH>::setTo(const Mat<HEIGHT, WIDTH>& other)
    {
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                data[i][j] = other.get(i, j);
    }

    template<int HEIGHT, int WIDTH>
    const Mat<HEIGHT, WIDTH>& Mat<HEIGHT, WIDTH>::operator=(const Mat<HEIGHT, WIDTH>& other)
    {
        setTo(other);
        return *this;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>::Mat(const Mat<HEIGHT, WIDTH>& other)
        : Mat()
    {
        setTo(other);
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> operator+(const Mat<HEIGHT, WIDTH>& first, const Mat<HEIGHT, WIDTH>& second)
    {
        return add(first, second);
    }
    
    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> operator-(const Mat<HEIGHT, WIDTH>& first, const Mat<HEIGHT, WIDTH>& second)
    {
        return subtract(first, second);
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> operator*(const Mat<HEIGHT, WIDTH>& mtx, float scalar)
    {
        return multiply(mtx, scalar);
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> operator*(float scalar, const Mat<HEIGHT, WIDTH>& mtx)
    {
        return multiply(mtx, scalar);
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> operator/(const Mat<HEIGHT, WIDTH>& mtx, float scalar)
    {
        return multiply(mtx, 1.f/scalar);
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> operator-(const Mat<HEIGHT, WIDTH>& mtx)
    {
        return multiply(mtx, -1.f);
    }

    template<int FIRST_HEIGHT, int FIRST_WIDTH, int SECOND_WIDTH>
    Mat<FIRST_HEIGHT, SECOND_WIDTH> operator*(const Mat<FIRST_HEIGHT, FIRST_WIDTH>& first, const Mat<FIRST_WIDTH, SECOND_WIDTH>& second)
    {
        return multiply(first, second);
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>& operator+=(Mat<HEIGHT, WIDTH>& first, const Mat<HEIGHT, WIDTH>& second)
    {
        first.add(second);
        return first;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>& operator-=(Mat<HEIGHT, WIDTH>& first, const Mat<HEIGHT, WIDTH>& second)
    {
        first.subtract(second);
        return first;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>& operator*=(Mat<HEIGHT, WIDTH>& mtx, float scalar)
    {
        mtx.multiply(scalar);
        return mtx;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH>::Mat()
    {
        assert(HEIGHT > 0);
        assert(WIDTH > 0);
    }

    template<int HEIGHT, int WIDTH>
    void Mat<HEIGHT, WIDTH>::setToIdentity()
    {
        assert(HEIGHT == WIDTH); // (probably) can't have non square identity matrices
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                if(i!=j)
                    data[i][j] = 0;
                else data[i][j] = 1;
    }

    template<int HEIGHT, int WIDTH>
    void Mat<HEIGHT, WIDTH>::fill(float n)
    {
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                data[i][j] = n;
    }

    template <int HEIGHT, int WIDTH>
    void Mat<HEIGHT, WIDTH>::add(const Mat<HEIGHT, WIDTH>& other)
    {
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                data[i][j] += other.get(i, j);
    }

    template <int HEIGHT, int WIDTH>
    void Mat<HEIGHT, WIDTH>::subtract(const Mat<HEIGHT, WIDTH>& other)
    {
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                data[i][j] -= other.get(i, j);
    }

    template <int HEIGHT, int WIDTH>
    void Mat<HEIGHT, WIDTH>::multiply(float scalar)
    {
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                data[i][j] *= scalar;
    }

    template<int HEIGHT, int WIDTH>
    void Mat<HEIGHT, WIDTH>::randomize(float lowerLimit, float upperLimit)
    {
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                *getPtr(i, j) = random(lowerLimit, upperLimit);
    }
    
    template<int HEIGHT>
    float module(const Mat<HEIGHT, 1>& mtx)
    {
        float result = 0.f;
        for(int i=0; i<HEIGHT; i++)
            result += mtx.get(i, 0) * mtx.get(i, 0);

        return sqrt(result);
    }

    template<int HEIGHT, int WIDTH>
    Mat<WIDTH, HEIGHT> transpose(const Mat<HEIGHT, WIDTH>& mtx)
    {
        Mat<WIDTH, HEIGHT> result;

        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                *result.getPtr(j, i) = mtx.get(i, j);

        return result;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> add(const Mat<HEIGHT, WIDTH>& first, const Mat<HEIGHT, WIDTH>& second)
    {
        Mat<HEIGHT, WIDTH> result;

        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                *result.getPtr(i, j) = first.get(i, j) + second.get(i, j);

        return result;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> subtract(const Mat<HEIGHT, WIDTH>& first, const Mat<HEIGHT, WIDTH>& second)
    {
        Mat<HEIGHT, WIDTH> result;

        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                *result.getPtr(i, j) = first.get(i, j) - second.get(i, j);

        return result;
    }

    template<int HEIGHT, int WIDTH>
    Mat<HEIGHT, WIDTH> multiply(const Mat<HEIGHT, WIDTH>& mtx, float scalar)
    {

        Mat<HEIGHT, WIDTH> result;

        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                *result.getPtr(i, j) = mtx.get(i, j) * scalar;

        return result;
    }

    template<int FIRST_HEIGHT, int FIRST_WIDTH, int SECOND_WIDTH>
    Mat<FIRST_HEIGHT, SECOND_WIDTH> multiply(const Mat<FIRST_HEIGHT, FIRST_WIDTH>& first, const Mat<FIRST_WIDTH, SECOND_WIDTH>& second)
    {

        Mat<FIRST_HEIGHT, SECOND_WIDTH> result;

        for(int i=0; i<FIRST_HEIGHT; i++)
            for(int j=0; j<SECOND_WIDTH; j++)
            {
                *result.getPtr(i, j) = 0;
                for(int k=0; k<FIRST_WIDTH; k++)
                    *result.getPtr(i, j) += first.get(i, k) * second.get(k, j);
            }

        return result;
    }

    class MatHeap // NOTE: dynamically alocated
    {
    public:
        MatHeap(int height, int width);
        MatHeap(int height, int width, float fillInValue);
        ~MatHeap();

        // not tested
        MatHeap(const MatHeap& other);

        MatHeap& operator=(const MatHeap& other) = delete;
        MatHeap& operator==(const MatHeap& other) = delete;

        void setToIdentity();
        void setTo(const MatHeap& other);

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

    template<int FIRST_HEIGHT, int FIRST_WIDTH, int SECOND_HEIGHT, int SECOND_WIDTH>
    void solveGaussSeidel(Mat<FIRST_HEIGHT, FIRST_WIDTH>* pX, const Mat<SECOND_HEIGHT, SECOND_WIDTH>& a,    // solved Ax=b with gauss seidel. can't solve all matrices
        const Mat<FIRST_HEIGHT, SECOND_WIDTH>& b, int iterationLimit)
    {
        assert(pX->width == 1); // x is not a vector

        assert(a.width == a.height); // a needs to be a square matrix for there to be an inverse matrix

        for(int iteration = 0; iteration < iterationLimit; iteration++)
        {
            for(int i=0; i<pX->height; i++)
            {
                if(a.get(i, i) == 0.f)
                    continue;

                float sum=0;
                for(int j=0; j<pX->height; j++)
                    //if(i != j)
                    sum += a.get(i, j) * pX->get(j, 0);

                *pX->getPtr(i, 0) += (b.get(i, 0) - sum) / a.get(i, i);
            }

            // TODO: handle this if
/*
            if(0)
            {
                cout << "Gauss-Siedel iteration: " << iteration << "/" << iterationLimit << endl;

                Mat<b.getHeight(), b.getWidth()> result = a * (*pX);
                multiply(&result, a, *pX);

                Mat<b.getHeight(), b.getWidth()> difference = b - result;
                float percentDifference = module(difference) / module(b);

                cout << "percentDifference: " << percentDifference << endl;
            }
*/
        }
    }

    void debugGaussSeidelTest();

    template<int HEIGHT, int WIDTH>
    bool compare(const Mat<HEIGHT, WIDTH>& first, const Mat<HEIGHT, WIDTH>& second, float marginOfError)
    {
        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<WIDTH; j++)
                if(abs(first.get(i, j) - second.get(i, j)) > marginOfError)
                    return false;
        return true;
    }

    template<int HEIGHT>
    Mat<HEIGHT, HEIGHT> scale(const Mat<HEIGHT, 1>& vec)
    {            
        Mat<HEIGHT, HEIGHT> result;

        for(int i=0; i<HEIGHT; i++)
            for(int j=0; j<HEIGHT; j++)
                if(i != j)
                    *result.getPtr(i, j) = 0.f;
                else *result.getPtr(i, i) = vec.get(i, 0);

        return result;
    }

    template<int HEIGHT>
    float dot(const Mat<HEIGHT, 1>& first, const Mat<HEIGHT, 1>& second)
    {
        float result = 0.f;
        for(int i=0; i<HEIGHT; i++)
            result += first.get(i, 0) * second.get(i, 0);

        return result;
    }

    float module(const MatHeap& mtx);
    void add(MatHeap* pResult, const MatHeap& first, const MatHeap& second);
    void subtract(MatHeap* pResult, const MatHeap& first, const MatHeap& second);
    void multiply(MatHeap* pResult, const MatHeap& first, const MatHeap& second);
    void multiply(MatHeap* pResult, const MatHeap& mtx, float scalar);

    void transpose(MatHeap* pResult, const MatHeap& mtx);
    void inverse(MatHeap* pResult, const MatHeap& mtx);



/*
    Quaternion conjugate(Quaternion quaternion);
    float moduleSquared(Quaternion quaternion);
    Quaternion inverse(Quaternion quaternion);
    Quaternion operator*(Quaternion quaternion, float scalar);
*/

}

using namespace mat;

void log(const MatHeap& thing);

//void log(const bool& thing);
//void log(const int thing);
//void log(const Rotation& thing);

template<int HEIGHT, int WIDTH>
void log(const Mat<HEIGHT, WIDTH>& mtx)
{
    if(WIDTH == 1)
    {
        if(HEIGHT == 1)
            cout << "x: " << mtx.get(0, 0) << endl;
        else if(HEIGHT == 2)
            cout << "x: " << mtx.get(0, 0) << " y: " << mtx.get(1, 0) << endl;
        else if(HEIGHT == 3)
            cout << "x: " << mtx.get(0, 0) << " y: " << mtx.get(1, 0) << " z: " << mtx.get(2, 0) << endl;
        else if(HEIGHT == 4)
            cout << "x: " << mtx.get(0, 0) << " y: " << mtx.get(1, 0) << " z: " << mtx.get(2, 0) << " w: " << mtx.get(3, 0) << endl;
        return ;
    }
    for(int i=0; i<HEIGHT; i++)
    {
        for(int j=0; j<WIDTH; j++)
            cout << mtx.get(i, j) << " ";
        cout << endl;
    }
}
