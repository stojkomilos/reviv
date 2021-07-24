#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"shader.h"
#include"buffer.h"

extern unsigned int ShaderDataTypeSize(ShaderDataType type);

struct MaterialHelpingStruct
{
    void setTo(const MaterialHelpingStruct& other)
    {
        RV_ASSERT(ptr == nullptr, "");
        type = other.type;
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
                RV_ASSERT(false, "ERROR: Shader data type not defined in switch statement");
                break;
        }
    }

    MaterialHelpingStruct() : ptr(nullptr) {}
    void* ptr = nullptr;
    ShaderDataType type;

    MaterialHelpingStruct(const MaterialHelpingStruct&) = delete;
    MaterialHelpingStruct& operator=(const MaterialHelpingStruct&) = delete;

    bool operator==(const MaterialHelpingStruct& other) const
    {
        RV_ASSERT(false, "")
        //return this->type == other.type && this->ptr == other.ptr;
        return (this->type == other.type && this->ptr == other.ptr);
    }

    bool operator!=(const MaterialHelpingStruct& other) const
    {
        RV_ASSERT(false, "")
        return !(*this == other);
    }

};

class Material                                      //TODO: destructor, prodji kroz unordered_map i brisi sve (pise tamo i koji je tip podatka da zna kako da obrise)
{                                                   //TODO: deep copy constructor?
public:                                             //TODO: destruktor, ima dosta, jer imam mapu sa new stvarima, treba pozvati "delete" svakog key-a
    Material() = default;
    Material(const Material& other)
    {
        RV_ASSERT(false, "");
    }
    Material& operator=(const Material& other)
    {
        RV_ASSERT(false, "");
    }
    Material(Material&& other)
    {
        RV_ASSERT(false, "");
    }

    Material(Shader* pShader)
        : pShader(pShader) { map.clear(); name.clear(); }

    void setShader(Shader* inShader);
    void bindShader();
    void bind() const;
    void set(const std::string& uniformName, const Mat4& mat4);
    void set(const std::string& uniformName, const Vec4f& vec4f);
    void set(const std::string& uniformName, const Vec3f& vec3f);
    void set(const std::string& uniformName, int n);

    Shader* pShader = nullptr;
    std::string name;
    std::unordered_map <std::string, MaterialHelpingStruct> map;
};

void log(const Material& material);
void logSpecificUniform(const Material& material, const std::string& uniformName);