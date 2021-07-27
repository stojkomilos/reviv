#include "texture.h"

void Texture::init()
{
    RV_ASSERT(isInited == false, "texture already loaded/inited");
    glGenTextures(1, &id);
    isInited = true;
}

void Texture2D::load(const std::string& filePath)
{
    RV_ASSERT(isInited == false, "texture already loaded/inited");
    isInited = true;

    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    RV_ASSERT(data, "Failed to load texture" << filePath);

    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);


    //GLenum internalFormat = 0, dataFormat = 0;
    //if (channels1 == 4)
    //{
    //    internalFormat = GL_RGBA8;
    //    dataFormat = GL_RGBA;
    //}
    //else if (channels1 == 3)
    //{
    //    internalFormat = GL_RGB8;
    //    dataFormat = GL_RGB;
    //}
    //RV_ASSERT(internalFormat & dataFormat, "Texture format not supported");

    //glCreateTextures(GL_TEXTURE_2D, 1, &id);
    //glTextureStorage2D(id, 1, internalFormat, width, height);

    //glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    //stbi_image_free(data);

}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int slot) const
{
    RV_ASSERT(slot < 16, "");
    RV_ASSERT(GL_TEXTURE1 == GL_TEXTURE0 + 1, "");

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(textureType, id);
    //glBindTextureUnit(slot, id);
}


/*
void Texture2D::load(const std::string& filePath)
{
    int width1, height1, channels1;
    stbi_uc* data = stbi_load(path.c_str(), &width1, &height1, &channels1, 0);
    stbi_set_flip_vertically_on_load(1);
    RV_ASSERT(data, "Failed to load texture" << path);
    width = width1; //TODO: clean
    height = height1;

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels1 == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (channels1 == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }
    RV_ASSERT(internalFormat & dataFormat, "Texture format not supported");

    glCreateTextures(GL_TEXTURE_2D, 1, &id);
    glTextureStorage2D(id, 1, internalFormat, width, height);

    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

}
*/