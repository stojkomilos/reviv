#pragma once

#include"rv_pch.hpp"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"core/mat.h"
#include"renderer/buffer.h"
#include"stls/stable_vector.h"

#include"renderer/texture.h"

//#include"renderer/shader_uniform_map.h"

class Shader{

public:
    Shader();
    unsigned int id = 0;
    void init(const char* vertexPath, const char* fragmentPath, const char* geometryShader = nullptr);
    void bind() const;

    int getUniformLocation(const std::string& uniformName) const;
    void uploadUniformMat4(const std::string& name, const Mat4& matrix) const;
    void uploadUniform3f(const std::string& name, const Vec3f& a) const; //mogu se optimizovati koristecu drugu glUniform funkciju
    void uploadUniform4f(const std::string& name, const Vec4f& a) const;
    void uploadUniform1i(const std::string& name, int a) const;
    void uploadUniform1f(const std::string& name, float a) const;

    std::string filePathVertex;
    std::string filePathFragment;

    stls::StableVector<std::string> uniformNames;

    unsigned char lastUsed = 0;

    unsigned int textureUniformCounter = 0; // always bind enviroment first, and then after bind material
    
private:
    void checkCompileErrors(GLuint shader, std::string type);
    int maxUniformNameLength = 0;
};

struct ShaderUniformHelpingStruct
{
    ShaderUniformHelpingStruct() : ptr(nullptr) {}
    void* ptr = nullptr;
    ShaderDataType type;

    ShaderUniformHelpingStruct(const ShaderUniformHelpingStruct&) = delete;
    ShaderUniformHelpingStruct& operator=(const ShaderUniformHelpingStruct&) = delete;

    bool operator==(const ShaderUniformHelpingStruct& other) const = delete;
    bool operator!=(const ShaderUniformHelpingStruct& other) const = delete;
};