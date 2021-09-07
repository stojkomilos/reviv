#include"shader_uniform_map.h"

ShaderUniformMap::~ShaderUniformMap()
{
    for(const auto& [uniformName, help] : map)
    {
        //delete help.ptr; //TODO: -Wdelete-incomplete: "warning: deleting 'void* const' is undefined" TODO:
    }
}

void ShaderUniformMap::set(const std::string& uniformName, const Vec3f& vec3f)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Vec3f(vec3f);
        foo->type = ShaderDataType::SdtFloat3;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat3, "");
        *(Vec3f*)it->second.ptr = vec3f;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, const Vec4f& vec4f)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Vec4f(vec4f);
        foo->type = ShaderDataType::SdtFloat4;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat4, "");
        *(Vec4f*)it->second.ptr = vec4f;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, unsigned int n)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new unsigned int;
        *(unsigned int*)foo->ptr = n;
        foo->type = ShaderDataType::SdtUint1;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtUint1, "");
        *(unsigned int*)it->second.ptr = n;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, int n)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new int;
        *(int*)foo->ptr = n;
        foo->type = ShaderDataType::SdtInt1;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtInt1, "");
        *(int*)it->second.ptr = n;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, float n)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new float;
        *(float*)foo->ptr = n;
        foo->type = ShaderDataType::SdtFloat1;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat1, "");
        *(float*)it->second.ptr = n;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, double n)
{
    RV_ASSERT(false, "seting doubles is disabled (for safety)");
}

void log(const ShaderUniformMap& shaderUniformMap)
{
    cout << "Uniforms:----" << endl;
    for(const auto& [key, value] : shaderUniformMap.map)
    {
        logSpecificUniform(shaderUniformMap, key);
    }

    cout << "Textures:----" << endl;
    for(const auto& [key, value] : shaderUniformMap.textureMap)
    {
        cout << key << " : " << value << endl;
    }
}

void logSpecificUniform(const ShaderUniformMap& shaderUniformMap, const std::string& uniformName)
{
    auto fromMap = shaderUniformMap.map.find(uniformName);
    if(fromMap == shaderUniformMap.map.end())
    {
        cout << uniformName << ": UNIFORM NOT DEFINED" << endl;
        RV_ASSERT(false, "");
        return;
    }
    const ShaderUniformHelpingStruct* value = &fromMap->second;

    cout << uniformName << ": " << endl;
    switch(value->type)
    {
        case ShaderDataType::SdtFloat1:           log(*(Vec1f*)value->ptr);      break;
        case ShaderDataType::SdtFloat2:          log(*(Vec2f*)value->ptr);      break;
        case ShaderDataType::SdtFloat3:          log(*(Vec3f*)value->ptr);     break;
        case ShaderDataType::SdtFloat4:         log(*(Vec4f*)value->ptr);     break;
        case ShaderDataType::SdtMat3:            log(*(Mat3*)value->ptr);       break;
        case ShaderDataType::SdtMat4:            log(*(Mat4*)value->ptr);       break;
        case ShaderDataType::SdtInt1:             log(*(int*)value->ptr);       break;
        case ShaderDataType::SdtBool:            log(*(bool*)value->ptr);       break;
        default: RV_ASSERT(false, "ERROR: ShaderUniformHelpingStruct data type not defined in log function");
    }
    cout << endl;
}

void ShaderUniformMap::uploadUniform(const Shader& shader, const std::string& existingUniformName) const
{
    shader.bind();

    ShaderUniformHelpingStruct help;

    auto iterator = map.find(existingUniformName);
    RV_ASSERT(iterator != map.end(), "uniform does not exist in ShaderUniformMap already");

    help.ptr = iterator->second.ptr;
    help.type = iterator->second.type;

    //cout << "MATERIAL=============" << endl;
    switch(help.type)
    {
        case ShaderDataType::SdtMat4:
            shader.uploadUniformMat4(existingUniformName, *(Mat4*)(help.ptr));
            //cout << "material uniform: " << uniformName;
            //log(*(Mat4*)(help.ptr));
            break;

        case ShaderDataType::SdtFloat1:
            shader.uploadUniform1f(existingUniformName, *(float*)(help.ptr));
            break;

        case ShaderDataType::SdtFloat3:
            shader.uploadUniform3f(existingUniformName, *(Vec3f*)(help.ptr));
            break;

        case ShaderDataType::SdtFloat4:
            shader.uploadUniform4f(existingUniformName, *(Vec4f*)(help.ptr));
            break;

        case ShaderDataType::SdtInt1:
            shader.uploadUniform1i(existingUniformName, *(int*)(help.ptr));
            break;

        default:
            RV_ASSERT(false, "ERROR: Specified uniform data type not found");
    }
}

void ShaderUniformMap::uploadAllUniforms(const Shader& shader) const
{
    shader.bind();

    ShaderUniformHelpingStruct help;
    for(const auto& [uniformName, help] : map)
    {
        uploadUniform(shader, uniformName);
        if(uniformName == "u_gPosition")
        {
            cout << "nah" << endl;
        }
    }
}

void ShaderUniformMap::set(const std::string& uniformName, const Mat4& mat4)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Mat4(mat4);
        foo->type = ShaderDataType::SdtMat4;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtMat4, "");
        *(Mat4*)it->second.ptr = mat4;
    }
}

void ShaderUniformMap::setTexture(const std::string& textureUniformName, const Texture& texture)
{
    textureMap[textureUniformName] = &texture;
}