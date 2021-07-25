#include "texture.h"

unsigned int Texture2D::getHeight() const
{
    return height;
}

unsigned int Texture2D::getWidth() const
{
    return width;
}

void Texture2D::init(const std::string& filePath)
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

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &id);
}

void Texture2D::bind(unsigned int slot) const
{
    glBindTextureUnit(slot, id);
}
