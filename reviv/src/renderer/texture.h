#pragma once

#include<glad/glad.h>
#include<stb_image.h>

#include"rv_pch.hpp"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual void bind(unsigned int slot = 0) const = 0;
    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;
    unsigned int width;
    unsigned int height;
};

class Texture2D : public Texture
{
public:
    Texture2D() = default;
    virtual ~Texture2D();

    void init(const std::string& filePath);
    unsigned int getWidth() const override;
    unsigned int getHeight() const override;
    void bind(unsigned int slot = 0) const override;

    std::string path;
    unsigned int width, height;
    unsigned int id;
};
