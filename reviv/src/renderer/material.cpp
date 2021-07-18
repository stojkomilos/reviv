#include"material.h"

void Material::bind() const
{
    pShader->bind();

    MaterialHelpingStruct help;
    std::string typeName;
    for(const auto& [uniformName, help] : map)
    {
        switch(help.type)
        {
            case ShaderDataType::SdtMat4:
                pShader->uploadUniformMat4(uniformName, *(Mat4*)(help.ptr));
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
    //pShader->bind(); //TODO ovo se mozda moze ukloniti
}

void Material::set(const std::string& uniformName, const Mat4& matrix)
{
    MaterialHelpingStruct help1;
    help1.type = ShaderDataType::SdtMat4; //TODOO destruktor za ovo i stvari oko ovoga
    help1.ptr = new Mat4;
    *(Mat4*)(help1.ptr) = matrix;
    map[uniformName] =  help1;
}

void Material::set(const std::string& uniformName, const Vec3f& a)
{
    MaterialHelpingStruct help1;
    help1.type = ShaderDataType::SdtFloat3;
    help1.ptr = new Vec3f;
    *(Vec3f*)(help1.ptr) = a;
    map[uniformName] =  help1;
}
void Material::set(const std::string& uniformName, const Vec4f& a)
{
    MaterialHelpingStruct help1;
    help1.type = ShaderDataType::SdtFloat4;
    help1.ptr = new Vec4f;
    *(Vec4f*)(help1.ptr) = a;
    map[uniformName] = help1;
}
void Material::set(const std::string& uniformName, int a)
{
    MaterialHelpingStruct help1;
    help1.type = ShaderDataType::SdtInt1;
    help1.ptr = new int;
    *(Vec1i*)help1.ptr = a;
    map[uniformName] =  help1;
}

void log(const Material& material)
{
    for(auto [key, value] : material.map)
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
        return;
    }
    MaterialHelpingStruct value = fromMap->second;

    cout << uniformName << ": " << endl;
    switch(value.type)
    {
        case ShaderDataType::SdtFloat1:           log(*(Vec1f*)value.ptr);      break;
        case ShaderDataType::SdtFloat2:          log(*(Vec2f*)value.ptr);      break;
        case ShaderDataType::SdtFloat3:          log(*(Vec3f*)value.ptr);     break;
        case ShaderDataType::SdtFloat4:         log(*(Vec4f*)value.ptr);     break;
        case ShaderDataType::SdtMat3:            log(*(Mat3*)value.ptr);       break;
        case ShaderDataType::SdtMat4:            log(*(Mat4*)value.ptr);       break;
        case ShaderDataType::SdtInt1:             log(*(Vec1i*)value.ptr);       break;
        case ShaderDataType::SdtInt2:            log(*(Vec2i*)value.ptr);       break;
        case ShaderDataType::SdtInt3:            log(*(Vec3i*)value.ptr);       break;
        case ShaderDataType::SdtInt4:            log(*(Vec4i*)value.ptr);       break;
        case ShaderDataType::SdtBool:            log(*(bool*)value.ptr);       break;
        default: RV_ASSERT(false, "ERROR: material data type not defined in log function");
    }
    cout << endl;
}