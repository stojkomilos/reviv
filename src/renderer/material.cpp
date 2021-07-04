#include"material.h"
#include<iostream>

void Material::bind() const
{
    shader.bind();

    MaterialHelpingStruct help;
    std::string typeName;
    for(const auto& [uniformName, help] : map)
    {
        switch(help.type)
        {
            case UniformDataType::UniformDataTypeMat4:
                shader.uploadUniformMat4(uniformName, *(Mat4*)(help.ptr));
                break;

            case UniformDataType::UniformDataTypeVec3f:
                shader.uploadUniform3f(uniformName, *(Vec3f*)(help.ptr));
                break;

            case UniformDataType::UniformDataTypeVec4f:
                shader.uploadUniform4f(uniformName, *(Vec4f*)(help.ptr));
                break;

            case UniformDataType::UniformDataTypeIntiger1:
                shader.uploadUniform1i(uniformName, *(int*)(help.ptr));
                break;

            default:
                std::cout << "ERROR: Specified uniform data type not found";
        }
    }
}

void Material::set(const std::string& uniformName, const Mat4& matrix)
{
    MaterialHelpingStruct help1;
    help1.type = UniformDataType::UniformDataTypeMat4;
    help1.ptr = new Mat4;
    *(Mat4*)(help1.ptr) = matrix;
    map[uniformName] =  help1;
}

void Material::set(const std::string& uniformName, const Vec3f& a)
{
    MaterialHelpingStruct help1;
    help1.type = UniformDataType::UniformDataTypeVec3f;
    help1.ptr = new Vec3f;
    *(Vec3f*)(help1.ptr) = a;
    map[uniformName] =  help1;
}
void Material::set(const std::string& uniformName, const Vec4f& a)
{
    MaterialHelpingStruct help1;
    help1.type = UniformDataType::UniformDataTypeVec4f;
    help1.ptr = new Vec4f;
    *(Vec4f*)(help1.ptr) = a;
    map[uniformName] =  help1;
}
void Material::set(const std::string& uniformName, int a)
{
    MaterialHelpingStruct help1;
    help1.type = UniformDataType::UniformDataTypeIntiger1;
    help1.ptr = new int;
    *(int*)(help1.ptr) = a;
    map[uniformName] =  help1;
}


//void Material::set(const std::string& uniformName, const Mat4& matrix) const
//{
//    shader.uploadUniformMat4(uniformName, matrix);
//}
//void Material::set(const std::string& uniformName, const Vec3f& a) const
//{
//    shader.uploadUniformVec3f(uniformName, a);
//}
//void Material::set(const std::string& uniformName, const Vec4f& a) const
//{
//    shader.uploadUniform4f(name, a);
//}
//void Material::set(const std::string& uniformName, int a) const
//{
//    shader.uploadUniform1i(name, a);
//}

// -----
