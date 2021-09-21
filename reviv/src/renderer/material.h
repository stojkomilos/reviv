#pragma once

#include"rv_pch.hpp"

#include"core/mat.h"
#include"shader.h"
#include"buffer.h"
#include"texture.h"
#include"shader_uniform_map.h"

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

    void reset(Shader* inShader); // sets shader to the new one and deletes the ShaderUniformMap

    inline void setTexture(const std::string& textureUniformName, const Texture& texture) { shaderUniformMap.setTexture(textureUniformName, texture); }

    inline void set(const std::string& uniformName, const Mat<4,4>& mat4) { shaderUniformMap.set(uniformName, mat4); }
    inline void set(const std::string& uniformName, const Vec4& vec4) { shaderUniformMap.set(uniformName, vec4); }
    inline void set(const std::string& uniformName, const Vec3& vec3) { shaderUniformMap.set(uniformName, vec3); }
    inline void set(const std::string& uniformName, int n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, float n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, double n) { shaderUniformMap.set(uniformName, n); }
    //inline void set(const std::string& uniformName, unsigned int n) { shaderUniformMap.set(uniformName, n); }

    Shader* pShader = nullptr;
    ShaderUniformMap shaderUniformMap;
private:
    void bind(); // NOTE: don't call bind() on it's own. You MUST use the RenderManager wrappers
    friend class RenderManager;
};

void log(const Material& material);