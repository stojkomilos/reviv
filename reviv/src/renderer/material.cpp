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

    //RV_ASSERT(pShader->environmentTextureUniformCounter > 0, "potential error: environment has not set any textures so far. Maybe you forgot that you HAVE to bind an environment before you bind a material? Maybe this is not an error");

    pShader->materialTextureUniformCounter = pShader->environmentTextureUniformCounter + 1;

    for(auto const& it : shaderUniformMap.textureMap)
    {
        //pShader->uploadUniform1i(textureUniformNames[i], i);
        set(it.first, (int)pShader->materialTextureUniformCounter);
        it.second->bind((int)pShader->materialTextureUniformCounter);
        
        pShader->materialTextureUniformCounter++;
    }

    shaderUniformMap.uploadAllUniforms(*pShader);
}

void log(const Material& material)
{
    log(material.shaderUniformMap);
}