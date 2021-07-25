#include"material.h"

Material::~Material()
{
    for(const auto& [uniformName, help] : map)
    {
        //delete help.ptr; //TODO: -Wdelete-incomplete: "warning: deleting 'void* const' is undefined" TODO:
    }
}

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

void Material::bind() const
{
    pShader->bind();

    MaterialHelpingStruct help;
    std::string typeName;
    //cout << "MATERIAL=============" << endl;
    for(const auto& [uniformName, help] : map)
    {
        switch(help.type)
        {
            case ShaderDataType::SdtMat4:
                pShader->uploadUniformMat4(uniformName, *(Mat4*)(help.ptr));
                //cout << "material uniform: " << uniformName;
                //log(*(Mat4*)(help.ptr));
                break;

            case ShaderDataType::SdtFloat3:
                pShader->uploadUniform3f(uniformName, *(Vec3f*)(help.ptr));
                break;

            case ShaderDataType::SdtFloat4:
                pShader->uploadUniform4f(uniformName, *(Vec4f*)(help.ptr));
                break;

            case ShaderDataType::SdtInt1:
                pShader->uploadUniform1i(uniformName, *(int*)(help.ptr));
                break;

            default:
                RV_ASSERT(false, "ERROR: Specified uniform data type not found");
        }
    }
    //cout << "ENDDDDDD MATERIAL--" << endl;
    pShader->bind(); //TODO ovo se mozda moze ukloniti
}

void Material::set(const std::string& uniformName, const Mat4& mat4)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        MaterialHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Mat4(mat4);
        foo->type = ShaderDataType::SdtMat4;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtMat4, "");
        *(Mat4*)it->second.ptr = mat4;
    }
}


void Material::set(const std::string& uniformName, const Vec3f& vec3f)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        MaterialHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Vec3f(vec3f);
        foo->type = ShaderDataType::SdtFloat3;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat3, "");
        *(Vec3f*)it->second.ptr = vec3f;
    }
}

void Material::set(const std::string& uniformName, const Vec4f& vec4f)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        MaterialHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Vec4f(vec4f);
        foo->type = ShaderDataType::SdtFloat4;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat4, "");
        *(Vec4f*)it->second.ptr = vec4f;
    }
}

void Material::set(const std::string& uniformName, int n)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        MaterialHelpingStruct* foo = &map[uniformName];
        foo->ptr = new int;
        *(int*)foo->ptr = n;
        foo->type = ShaderDataType::SdtInt1;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtInt1, "");
        *(int*)it->second.ptr = n;
    }
}

void log(const Material& material)
{
    for(const auto& [key, value] : material.map)
    {
        logSpecificUniform(material, key);
    }
}

void logSpecificUniform(const Material& material, const std::string& uniformName)
{
    auto fromMap = material.map.find(uniformName);
    if(fromMap == material.map.end())
    {
        cout << uniformName << ": UNIFORM NOT DEFINED" << endl;
        RV_ASSERT(false, "");
        return;
    }
    const MaterialHelpingStruct* value = &fromMap->second;

    cout << uniformName << ": " << endl;
    switch(value->type)
    {
        case ShaderDataType::SdtFloat1:           log(*(Vec1f*)value->ptr);      break;
        case ShaderDataType::SdtFloat2:          log(*(Vec2f*)value->ptr);      break;
        case ShaderDataType::SdtFloat3:          log(*(Vec3f*)value->ptr);     break;
        case ShaderDataType::SdtFloat4:         log(*(Vec4f*)value->ptr);     break;
        case ShaderDataType::SdtMat3:            log(*(Mat3*)value->ptr);       break;
        case ShaderDataType::SdtMat4:            log(*(Mat4*)value->ptr);       break;
        case ShaderDataType::SdtInt1:             log(*(Vec1i*)value->ptr);       break;
        case ShaderDataType::SdtInt2:            log(*(Vec2i*)value->ptr);       break;
        case ShaderDataType::SdtInt3:            log(*(Vec3i*)value->ptr);       break;
        case ShaderDataType::SdtInt4:            log(*(Vec4i*)value->ptr);       break;
        case ShaderDataType::SdtBool:            log(*(bool*)value->ptr);       break;
        default: RV_ASSERT(false, "ERROR: material data type not defined in log function");
    }
    cout << endl;
}