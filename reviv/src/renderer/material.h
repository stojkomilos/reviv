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

    Material(Shader* pShader)
        : pShader(pShader) {}

    void setShader(Shader* inShader);
    void bindShader();
    void bind();

    inline void setTexture(const std::string& textureUniformName, const Texture& texture) { shaderUniformMap.setTexture(textureUniformName, texture); }

    inline void set(const std::string& uniformName, const Mat4& mat4) { shaderUniformMap.set(uniformName, mat4); }
    inline void set(const std::string& uniformName, const Vec4f& vec4f) { shaderUniformMap.set(uniformName, vec4f); }
    inline void set(const std::string& uniformName, const Vec3f& vec3f) { shaderUniformMap.set(uniformName, vec3f); }
    inline void set(const std::string& uniformName, int n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, float n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, double n) { shaderUniformMap.set(uniformName, n); }

    Shader* pShader = nullptr;
    ShaderUniformMap shaderUniformMap;
};

void log(const Material& material);