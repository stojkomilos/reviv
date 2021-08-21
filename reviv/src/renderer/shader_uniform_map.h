#pragma once

#include"shader.h"

class ShaderUniformMap
{
public:
    ShaderUniformMap() { map.clear(); }
    ~ShaderUniformMap();

    std::unordered_map<std::string, const Texture*> textureMap; // first = texture uniform name ; second = pointer to the actual texture object

    void setTexture(const std::string& textureUniformName, const Texture& texture);

    void uploadUniform(const Shader& shader, const std::string& existingUniformName) const;
    void uploadAllUniforms(const Shader& shader) const;
    std::unordered_map <std::string, ShaderUniformHelpingStruct> map;
    void set(const std::string& uniformName, const Mat4& mat4);
    void set(const std::string& uniformName, const Vec4f& vec4f);
    void set(const std::string& uniformName, const Vec3f& vec3f);
    void set(const std::string& uniformName, int n);
    void set(const std::string& uniformName, unsigned int n);
    void set(const std::string& uniformName, float n);
    void set(const std::string& uniformName, double n);
};

void log(const ShaderUniformMap& shaderUniformMap);
void logSpecificUniform(const ShaderUniformMap& shaderUniformMap, const std::string& uniformName);