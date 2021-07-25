#pragma once

#include"rv_pch.hpp"

#include"stls/mat.h"

class Light
{
public:
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;

    bool on = true;
    float intensity = 1.0f;
};

class PointLight : public Light
{
};

class DirectionalLight : public Light
{
    Vec3f direction;
};