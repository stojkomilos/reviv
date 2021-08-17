#pragma once

#include"texture.h"
#include"buffer.h"
#include"shader.h"
#include"framebuffer.h"

class Deffered
{
public:
    void init();

    Shader geometryPassShader;
    Framebuffer gBuffer;
    Texture2D gPosition, gNormal, gAlbedoSpecular;
    Texture2D gDepth;
};
