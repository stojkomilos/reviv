#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include<assert.h>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"reviv/core/mat.h"

bool GLLogCall();
void GLClearError();

class Shader{

public:
    Shader();
    unsigned int ID;
    void setUp(const char* vertexPath, const char* fragmentPath); ///, const char* geometryPath
    void bind();

    void uploadUniformMat4(const std::string& name, mat::mat4& matrix);
    void uploadUniform3f(const std::string& name, mat::vec3 a); //mogu se optimizovati koristecu drugu glUniform funkciju
    void uploadUniform4f(const std::string& name, mat::vec4 a);
    void uploadUniform1i(const std::string& name, int a);
private:
    void checkCompileErrors(GLuint shader, std::string type);
};