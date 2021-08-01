#pragma once

#include<glad/glad.h>

#include"texture.h"

struct FramebufferSpecification
{
    unsigned int width, height;
    unsigned int Samples = 1;

    bool swapChainTarget = false;
};

class Framebuffer 
{
public:
    Framebuffer() = default;
    ~Framebuffer();

    void init();
    void bind();
    void unbind();

    void attachTexture();
    void attachRenderbuffer();

    unsigned int id;

    bool isInited = false;
    Texture* pTexture;
    unsigned int rbo;
    //unsigned int colorAttachment, depthAttachment;
    //FramebufferSpecification specification;

    //void setUp(FramebufferSpecification& specification1);
    //void invalidate();
    //void unbind();
};