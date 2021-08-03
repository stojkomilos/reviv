#include"material.h"
#include"buffer.h"

void Material::addTexture(const std::string& textureUniformName, const Texture& texture)
{
#if RV_DEBUG
    for(int i=0; i<textureUniformNames.size(); i++)
        for(int j=0; j<textureUniformNames.size(); j++)
        {
            if(i != j)
            {
                RV_ASSERT(textureUniformNames[i] != textureUniformNames[j], "can't have more than one texture with the same uniform name");
            }
        }
#endif

    pTextures.pushBack(&texture);
    textureUniformNames.pushBack(textureUniformName);
}

void Material::setShader(Shader* inShader)
{
    RV_ASSERT(pShader == nullptr, "");
    pShader = inShader;
    pTextures.reserve(10);
    textureUniformNames.reserve(10);
}

void Material::bindShader()
{
    RV_ASSERT(pShader != nullptr, "");
    pShader->bind();
}

void Material::bind()
{
    pShader->bind();

    int textureCounter=0;
    for(unsigned int i=0; i<pTextures.size(); i++) // TODO: zameniti "auto it =..." sa "range based for loop", e.g. "auto it : pTextures"
    {
        //pShader->uploadUniform1i(textureUniformNames[i], i);
        set(textureUniformNames[i], (int)i);
        pTextures[i]->bind(i);
    }

    shaderUniformMap.uploadAllUniforms(*pShader);
}

void log(const Material& material)
{
    log(material.shaderUniformMap);
}
