#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"shader.h"
#include"buffer.h"
#include"texture.h"

extern unsigned int ShaderDataTypeSize(ShaderDataType type);

class Material
{
public:
    Material() = default;
    ~Material() = default;
    Material(const Material& other) = delete; // can be implemented
    Material& operator=(const Material& other) = delete; // can be implemented

    Material(const Shader* pShader)
        : pShader(pShader), pTextures(10) {}

    void setShader(Shader* inShader);
    void bindShader();
    void bind() const;

    //void setTexture(const std::string& uniformName, unsigned int slot);

    const Shader* pShader = nullptr;
    stls::StableVector<const Texture*> pTextures;

    void addTexture(const Texture& texture);

    ShaderUniformMap shaderUniformMap;

    inline void set(const std::string& uniformName, const Mat4& mat4) { shaderUniformMap.set(uniformName, mat4); }
    inline void set(const std::string& uniformName, const Vec4f& vec4f) { shaderUniformMap.set(uniformName, vec4f); }
    inline void set(const std::string& uniformName, const Vec3f& vec3f) { shaderUniformMap.set(uniformName, vec3f); }
    inline void set(const std::string& uniformName, int n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, float n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, double n) { shaderUniformMap.set(uniformName, n); }
};

void log(const Material& material);