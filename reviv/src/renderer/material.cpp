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

    cout << "texture shader: " << pShader->filePathVertex << endl;
    for(auto const& it : shaderUniformMap.textureMap)
    {
        //pShader->uploadUniform1i(textureUniformNames[i], i);
        RV_ASSERT(it.second->isInited == true, "material setting a texture that is not inited");

        set(it.first, (int)pShader->textureUniformCounter);
        it.second->bind((int)pShader->textureUniformCounter);
        
        cout << "texture uniformName: " << it.first << " counter: " << pShader->textureUniformCounter << endl;

        pShader->textureUniformCounter++;
    }
    cout << "------";

    shaderUniformMap.uploadAllUniforms(*pShader);
}

void log(const Material& material)
{
    log(material.shaderUniformMap);
}