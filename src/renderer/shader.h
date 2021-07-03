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
    void setUp(const char* vertexPath, const char* fragmentPath); ///, const char* geometryPath
    void bind();

    void uploadUniformMat4(const std::string& name, Mat4& matrix);
    void uploadUniform3f(const std::string& name, Vec3f a); //mogu se optimizovati koristecu drugu glUniform funkciju
    void uploadUniform4f(const std::string& name, Vec4f a);
    void uploadUniform1i(const std::string& name, int a);
private:
    void checkCompileErrors(GLuint shader, std::string type);
};
