#pragma once

#include<glad/glad.h>

#include"texture.h"

class Framebuffer 
{
public:
    Framebuffer() = default;
    ~Framebuffer();

    void init();
    void bind();
    void unbind();

    unsigned int id;

    bool isInited = false;
    unsigned int rbo;
};