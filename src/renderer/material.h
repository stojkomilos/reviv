#pragma once

#include<string>
#include<unordered_map>

#include"core/mat.h"
#include"core/entity.h"
#include"core/basic_components.h"
#include"shader.h"

enum UniformDataType
{
    UniformDataTypeMat4 = 0, UniformDataTypeVec3f, UniformDataTypeVec4f, UniformDataTypeIntiger1
};
struct MaterialHelpingStruct
{
    void* ptr;
    UniformDataType type;
};

class Material : public SpecificComponent<Material> //TODO: destructor, prodji kroz unordered_map i brisi sve (pise tamo i koji je tip podatka da zna kako da obrise)
{                                                   //TODO: deep copy constructor?
public:
    Shader shader;
    std::string name;
    std::unordered_map <std::string, MaterialHelpingStruct> map;
    Material() = default;
    Material(const Material&) = default;
    Material(const Shader& shader)
        : shader(shader) {}

    void bind() const;
    void set(const std::string& uniformName, const Mat4& matrix);
    void set(const std::string& uniformName, const Vec3f& a);
    void set(const std::string& uniformName, const Vec4f& a);
    void set(const std::string& uniformName, int a);
};