#pragma once

#include<glad/glad.h>
#include<stb_image.h>

#include"rv_pch.hpp"

class Texture
{
public:
    virtual ~Texture();

    void init();
    void bind(unsigned int slot) const;

    const int textureType;

    int width;
    int height;
    int nrChannels;
    unsigned int id = 0;

    bool isInited = false;

    std::string m_FilePath;
    //unsigned int getWidth() const = 0;
    //unsigned int getHeight() const = 0;
protected:
    Texture() = delete;
    Texture(int textureType)
        : textureType(textureType) { }
};

class Texture2D : public Texture
{
public:
    Texture2D()
        : Texture(GL_TEXTURE_2D) { }

    void load(const std::string& filePath);

    //unsigned int getWidth() const override;
    //unsigned int getHeight() const override;
};


        //stanicTexture.setUp("../resources/textures/stene.png");
        //stanicTexture.bind(0);
        //beloTexture.setUp("../resources/textures/belo.png");

        //textureCube.init("assets/textures/container.png");