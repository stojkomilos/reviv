#pragma once

#include"core/mat.h"

class Perlin2D
{
public:
    Perlin2D() = default;
    ~Perlin2D();

    void init(unsigned int gridWidth, unsigned int gridHeight);
    float get(Vec2f position);

    int m_GridWidth, m_GridHeight;
    Vec2f* pGrid;

private:
    float dotGrid(const Vec2f& realPosition, int gridPositionX, int gridPositionY);
    Vec2f* getPointer(unsigned int iIndex, unsigned int jIndex);
    bool isInited = false;
    float interpolate(float a0, float a1, float w);
};
