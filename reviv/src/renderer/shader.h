#pragma once

#include"rv_pch.hpp"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"core/mat.h"

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
private:
    void checkCompileErrors(GLuint shader, std::string type);
};