#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"shader.h"
#include"buffer.h"

extern unsigned int ShaderDataTypeSize(ShaderDataType type);

struct MaterialHelpingStruct
{
    MaterialHelpingStruct(const MaterialHelpingStruct& other)
        : type(other.type)
    {
        switch(type)
        {
            case(ShaderDataType::SdtInt1):
                ptr = new Vec1i;        // TODOOO destruktor za ovo i stvari oko ovoga
                *(Vec1i*)ptr = *(Vec1i*)other.ptr; // TODO staviti ovu funkciju u neki .cpp fajl ili tako nesto
                break;
            case(ShaderDataType::SdtFloat3):
                ptr = new Vec3f;
                *(Vec3f*)ptr = *(Vec3f*)other.ptr;
                break;
            case(ShaderDataType::SdtFloat4):
                ptr = new Vec4f;
                *(Vec4f*)ptr = *(Vec4f*)other.ptr;
                break;
            case(ShaderDataType::SdtMat4):
                ptr = new Mat4;
                *(Mat4*)ptr = *(Mat4*)other.ptr;
                break;

            default:
                cout << "ERROR: Shader data type not defined in switch statement" << endl;
                break;
        }
    }

    MaterialHelpingStruct& operator=(const MaterialHelpingStruct& other) = default;
    //MaterialHelpingStruct& operator=(const MaterialHelpingStruct& other)
    //{
    //    assert(false);
    //}

    bool operator==(const MaterialHelpingStruct& other) const
    {
        RV_ASSERT(false, "")
        //return this->type == other.type and this->ptr == other.ptr;
        return (this->type == other.type and this->ptr == other.ptr);
    }

    bool operator!=(const MaterialHelpingStruct& other) const
    {
        RV_ASSERT(false, "")
        return !(*this == other);
    }

    MaterialHelpingStruct() : ptr(nullptr) {}
    void* ptr;
    ShaderDataType type;

};

class Material                                      //TODO: destructor, prodji kroz unordered_map i brisi sve (pise tamo i koji je tip podatka da zna kako da obrise)
{                                                   //TODO: deep copy constructor?
public:                                             //TODO: destruktor, ima dosta, jer imam mapu sa new stvarima, treba pozvati "delete" svakog key-a
    Material() = default;
    Material(const Material&) = default;
    Material(Shader* pShader)
        : pShader(pShader) { map.clear(); name.clear(); }

    void bind() const;
    void set(const std::string& uniformName, const Mat4& matrix);
    void set(const std::string& uniformName, const Vec3f& a);
    void set(const std::string& uniformName, const Vec4f& a);
    void set(const std::string& uniformName, int a);

    Shader* pShader = nullptr;
    std::string name;
    std::unordered_map <std::string, MaterialHelpingStruct> map;
};

void log(const Material& material);
void logSpecificUniform(const Material& material, const std::string& uniformName);