#include "texture.h"

void Texture::init()
{
    RV_ASSERT(isInited == false, "texture already loaded/inited");
    glGenTextures(1, &id);
    isInited = true;
}

void Texture2D::load(const std::string& filePath) // uses sRGB
{
    RV_ASSERT(isInited == false, "texture already loaded/inited");
    m_FilePath = filePath;
    isInited = true;

    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    RV_ASSERT(data, "Failed to load texture" << filePath);

    GLenum internalFormat = 0, dataFormat = 0;
    if (nrChannels == 4)
    {
        internalFormat = GL_SRGB_ALPHA;
        dataFormat = GL_RGBA;
    }
    else if (nrChannels == 3)
    {
        internalFormat = GL_SRGB;
        dataFormat = GL_RGB;
    }
    RV_ASSERT(internalFormat & dataFormat, "Texture format not supported");

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

Texture::~Texture()
{
    RV_ASSERT(isInited == true, "");
    glDeleteTextures(1, &id);
}

void Texture::bind(unsigned int slot) const
{
    RV_ASSERT(slot < 16, "");
    RV_ASSERT(GL_TEXTURE1 == GL_TEXTURE0 + 1, "");

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(textureType, id);
}

void Texture2D::initFramebuffer(int inWidth, int inHeight)
{
    RV_ASSERT(isInited == false, "");
    isInited = true;

    width = inWidth;
    height = inHeight;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureCubeMap::load(std::vector<std::string> filePaths) // loads in sRGB
{
    RV_ASSERT(isInited == false, "texture already loaded/inited");
    isInited = true;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);


    stbi_set_flip_vertically_on_load(true);
    for(int i=0; i < 6; i++)
    {
        int tempWidth, tempHeight, tempNrChannels;
        unsigned char* data = stbi_load(filePaths[i].c_str(), &tempWidth, &tempHeight, &tempNrChannels, 0);
        RV_ASSERT(data, "Failed to load a texture of type cubemap");
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB,          tempWidth, tempHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //TODO: positive X ne valja u mom koordinatnom sistemu
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
}