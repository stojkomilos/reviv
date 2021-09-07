#include"random.h"

Perlin2D::~Perlin2D()
{
    delete [] pGrid;
}

Vec2f* Perlin2D::getPointer(unsigned int iIndex, unsigned int jIndex)
{
    return pGrid + iIndex * m_GridWidth + jIndex;
}

void Perlin2D::init(unsigned int gridWidth, unsigned int gridHeight)
{
    RV_ASSERT(isInited == false, "");

    isInited = true;
    m_GridWidth = gridWidth;
    m_GridHeight = gridHeight;

    RV_ASSERT(m_GridWidth % 2 == 1 && m_GridHeight % 2 == 1, "grid width and height must be odd numbers");

    pGrid = new Vec2f[m_GridWidth * m_GridHeight];

    for(int i=0; i < m_GridHeight; i++)
        for(int j=0; j < m_GridWidth; j++)
        {
            float tempAngle = (rand() % 1000) / 999.f * 2 * 3.14f;
            *getPointer(i, j) = Vec2f(cos(tempAngle), sin(tempAngle));
        }
}

float Perlin2D::interpolate(float a0, float a1, float w)
{
    //return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
    return (a1 - a0) * w + a0;
}

float Perlin2D::dotGrid(const Vec2f& position, int gridPositionX, int gridPositionY)
{
    return dot(position - Vec2f(gridPositionX, gridPositionY), *getPointer(gridPositionY, gridPositionX));
}

float Perlin2D::get(Vec2f position)
{
    RV_ASSERT((position.a[0] < (m_GridWidth / 2)) && (position.a[0] > (-m_GridWidth / 2)), "")
    RV_ASSERT(position.a[0] < m_GridHeight / 2 && position.a[0] > -m_GridHeight / 2, "")
    RV_ASSERT(isInited == true, "");

    position.a[0] += m_GridWidth / 2;
    position.a[1] += m_GridHeight / 2;

    int xLess = (int)position.a[0];
    int xMore = xLess + 1;
    int yLess = (int)position.a[1];
    int yMore = yLess + 1;

    float result = 0;
    float weightX = position.a[0] - xLess;
    float weightY = position.a[1] - yLess;

    float n0 = dotGrid(position, xLess, yLess);
    float n1 = dotGrid(position, xMore, yLess);
    float tempResultLowY = interpolate(n0, n1, weightX);

    n0 = dotGrid(position, xLess, yMore);
    n1 = dotGrid(position, xMore, yMore);
    float tempResultHighY = interpolate(n0, n1, weightX);

    result = interpolate(tempResultLowY, tempResultHighY, weightY);

    return result;
}