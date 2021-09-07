#pragma once

#include"texture.h"
#include"buffer.h"
#include"shader.h"
#include"framebuffer.h"

#include"event/event.h"

class Deffered
{
public:
    void init(unsigned int gBufferWidth, unsigned int gBufferHeight);
    void initGBuffer(unsigned int gBufferWidth, unsigned int gBufferHeight);

    void resize(unsigned int gBufferWidth, unsigned int gBufferHeight);

    unsigned int m_Width;
    unsigned int m_Height;

    Shader geometryPassShader;
    Framebuffer gBuffer;
    Texture2D gPosition, gNormal, gAlbedoSpecular;
    Texture2D gDepth;

    unsigned int rboDepth;
};
