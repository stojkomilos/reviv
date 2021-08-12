#include"material.h"
#include"buffer.h"

void Material::setShader(Shader* inShader)
{
    RV_ASSERT(pShader == nullptr, "");
    pShader = inShader;
}

void Material::bindShader()
{
    RV_ASSERT(pShader != nullptr, "");
    pShader->bind();
}

void Material::bind()
{
    pShader->bind();
:wqa
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
