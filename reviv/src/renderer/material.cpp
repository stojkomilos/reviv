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

    for(auto const& it : shaderUniformMap.textureMap)
    {
        RV_ASSERT(it.second->isInited == true, "material setting a texture that is not inited");

        set(it.first, (int)pShader->textureUniformCounter);
        it.second->bind((int)pShader->textureUniformCounter);
        
        pShader->textureUniformCounter++;
    }

    shaderUniformMap.uploadAllUniforms(*pShader);
}

void log(const Material& material)
{
    log(material.shaderUniformMap);
}

void Material::reset(Shader* inShader)
{
    RV_ASSERT(pShader != nullptr && inShader != nullptr, "");

    pShader = inShader;

    shaderUniformMap.map.clear(); // TODO: there's probably a memory leak
    shaderUniformMap.textureMap.clear();
}