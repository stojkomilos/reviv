#pragma once

#include"core/mat.h"

class Perlin2D
{
public:
    Perlin2D() = default;
    ~Perlin2D();

    void init(unsigned int gridWidth, unsigned int gridHeight);
    float get(Vec2 position);

    int m_GridWidth, m_GridHeight;
    Vec2* pGrid;

private:
    float dotGrid(const Vec2& realPosition, int gridPositionX, int gridPositionY);
    Vec2* getPointer(unsigned int iIndex, unsigned int jIndex);
    bool isInited = false;
    float interpolate(float a0, float a1, float w);
};
