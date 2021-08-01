#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"framebuffer.h"

class Light
{
public:
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;

    bool on = true;
    float intensity = 1.0f;
protected:
    Light();
};

class PointLight : public Light
{
public:
    PointLight(); 
    float constant; // good ranges numbers for different ranges: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    float linear;
    float quadratic;
};

class DirectionalLight : public Light
{
public:
    Vec3f direction;
};

class ShadowMap
{
public:
    void init(unsigned int width, unsigned int height);

    Framebuffer framebuffer;
    Texture2D depthMap;

    unsigned int m_Width;
    unsigned int m_Height;
};

void log(const Light& light);
void log(const DirectionalLight& light);
void log(const PointLight& light);