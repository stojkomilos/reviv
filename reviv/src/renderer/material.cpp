#include"material.h"
#include"buffer.h"

void Material::addTexture(const Texture& texture)
{
    RV_ASSERT(texture.isInited == true, "");
    pTextures.pushBack(&texture);
}


void Material::setShader(Shader* inShader)
{
    RV_ASSERT(pShader == nullptr, "");
    pShader = inShader;
    pTextures.reserve(5);
}

void Material::bindShader()
{
    RV_ASSERT(pShader != nullptr, "");
    pShader->bind();
}

void Material::bind() const
{
    pShader->bind();
    shaderUniformMap.bind(*pShader);
    // TODO: textures stuff
}

//void Material::setTexture(const std::string& uniformName, unsigned int slot)
//{
//    set(uniformName, slot);
//}



void log(const Material& material)
{
    log(material.shaderUniformMap);
}
