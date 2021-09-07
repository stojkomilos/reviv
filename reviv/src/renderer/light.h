#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"framebuffer.h"
#include"camera.h"

enum LightType
{
    LightTypeDirectional = 0, LightTypePoint
};

class ShadowMap
{
public:
    virtual ~ShadowMap() = default;
    virtual void init(unsigned int resolutionWidth, unsigned int resolutionHeight) = 0;
    virtual Texture* getDepthMap() = 0;

    Framebuffer framebuffer;

    unsigned int m_ResolutionWidth;
    unsigned int m_ResolutionHeight;

    float width;

    float nearRenderDistance;
    float renderDistance;

    Camera camera;

protected:
    ShadowMap() = default;
};

class ShadowMapDirectional : public ShadowMap
{
public:
    virtual ~ShadowMapDirectional() = default;
    virtual void init(unsigned int resolutionWidth, unsigned int resolutionHeight) override;
    virtual Texture* getDepthMap() override;
    Texture2D depthMap;
};

class ShadowMapOmnidirectional : public ShadowMap
{
public:
    virtual ~ShadowMapOmnidirectional() = default;
    virtual void init(unsigned int resolutionWidth, unsigned int resolutionHeight) override;
    virtual Texture* getDepthMap() override;
    Texture2D depthMap;
    TextureCubeMap shadowMap;
};

class Light
{
public:
    virtual ~Light() = default;

    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;

    bool on = true;
    float intensity = 1.0f;

    virtual ShadowMap* getShadowMap() = 0; // TODO: try to "inline" this function
    bool isShadowMapped = false;

    virtual void enableShadowMap() = 0;

    const LightType lightType;

protected:
    Light(LightType lightType)
        : lightType(lightType) // TODO: pokusati staviti ove ostale stvari u member initazlier list, i onda sve to mozda u cpp file
    {
        ambient = Vec3f(0.1f, 0.1f, 0.1f);
        diffuse = {0.2, 0.2, 0.2};
        specular = {0.5, 0.5, 0.5};
        on = true;
        intensity = 1.f;
    }
};

class PointLight : public Light
{
public:
    PointLight()
     : Light(LightTypePoint) // TODO: try to set constant linear and quadratic with member initializer list (with the addition of LightType)
    {
        constant = 1.f; // range approximately 32 meters
        linear = 0;
        quadratic = 0.07f;
    }
    virtual ~PointLight() = default;

    virtual void enableShadowMap() override;
    virtual ShadowMap* getShadowMap() override;

    float constant; // Invalid for gamma corrected? good ranges numbers for different ranges: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    float linear;
    float quadratic;

    ShadowMapOmnidirectional shadowMap;

};

class DirectionalLight : public Light
{
public:
    DirectionalLight()
     : Light(LightTypeDirectional) {}

    virtual ~DirectionalLight() = default;
    virtual void enableShadowMap() override;
    virtual ShadowMap* getShadowMap() override;

    ShadowMapDirectional shadowMap;
};


void log(const Light& light);
void log(const DirectionalLight& light);
void log(const PointLight& light);