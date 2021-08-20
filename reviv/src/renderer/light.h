#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"framebuffer.h"
#include"camera.h"

class ShadowMap
{
public:
    void init(unsigned int resolutionWidth, unsigned int resolutionHeight);

    Framebuffer framebuffer;
    Texture2D depthMap;

    unsigned int m_ResolutionWidth;
    unsigned int m_ResolutionHeight;

    float width;

    float nearRenderDistance;
    float renderDistance;

    Camera camera;
};

class Light
{
public:
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;

    bool on = true;
    float intensity = 1.0f;

    ShadowMap shadowMap;
    bool isShadowMapped = false;

    void enableShadowMap();

protected:
    Light();
};

class PointLight : public Light
{
public:
    PointLight(); 
    float constant; // Invalid for gamma corrected? good ranges numbers for different ranges: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    float linear;
    float quadratic;
};

class DirectionalLight : public Light
{
public:
};


void log(const Light& light);
void log(const DirectionalLight& light);
void log(const PointLight& light);