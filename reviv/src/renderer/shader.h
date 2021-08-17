#pragma once

#include"rv_pch.hpp"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"core/mat.h"
#include"renderer/buffer.h"
#include"stls/stable_vector.h"

#include"renderer/texture.h"

class Shader{

public:
    Shader();
    unsigned int id;
    void init(const char* vertexPath, const char* fragmentPath); ///, const char* geometryPath
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

    unsigned int environmentTextureUniformCounter = 0; // always bind enviroment first, and then after bind material
    unsigned int materialTextureUniformCounter = 0;
    
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
    void set(const std::string& uniformName, float n);
    void set(const std::string& uniformName, double n);
};

void log(const ShaderUniformMap& shaderUniformMap);
void logSpecificUniform(const ShaderUniformMap& shaderUniformMap, const std::string& uniformName);