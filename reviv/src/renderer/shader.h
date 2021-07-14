#pragma once

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<assert.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"core/mat.h"

bool GLLogCall();
void GLClearError();

class Shader{

public:
    Shader();
    unsigned int ID;
    void init(const char* vertexPath, const char* fragmentPath); ///, const char* geometryPath
    void bind() const;

    void uploadUniformMat4(const std::string& name, const Mat4& matrix) const;
    void uploadUniform3f(const std::string& name, const Vec3f& a) const; //mogu se optimizovati koristecu drugu glUniform funkciju
    void uploadUniform4f(const std::string& name, const Vec4f& a) const;
    void uploadUniform1i(const std::string& name, int a) const;
private:
    void checkCompileErrors(GLuint shader, std::string type);
};