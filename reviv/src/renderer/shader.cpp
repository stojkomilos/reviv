#include "shader.h"

Shader::Shader()
{ 
    id = 0;
};

void Shader::init(const char* vertexPath, const char* fragmentPath) { //, const char* geometryPath = nullptr)

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    //gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {

        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        //std::cout << vertexCode << std::endl << fragmentCode << std::endl;
       /* if (geometryPath != nullptr) {
            gShaderFile.open(geometryPath);
            std::stringstream  gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            geometryCode = gShaderStream.str();
            gShaderFile.close();
        }*/
    }

    catch (const std::ifstream::failure& e)
    {
        RV_ASSERT(false, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    /*unsigned int geometry;
    if (geometryPath != nullptr) {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }*/

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    //if (geometryPath != nullptr)
    //    glAttachShader(id, geometry);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  //  if (geometryPath != nullptr)
  //      glDeleteShader(geometry);
}
void Shader::bind() const
{
    glUseProgram(id);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            RV_ASSERT(false, "ERROR::SHADER_COMPILATION_ERROR of type: " << type << infoLog);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            RV_ASSERT(false, "ERROR::PROGRAM_LINKING_ERROR of type: " << type << infoLog);
        }
    }
}

int Shader::getUniformLocation(const std::string& uniformName) const
{
    int location = glGetUniformLocation(id, uniformName.c_str());
    RV_ASSERT(location != -1, "Retreiving the uniform location failed: This function returns -1 if name does not correspond to an active uniform variable in program, if name starts with the reserved prefix 'gl_', or if name is associated with an atomic counter or a named uniform block. See: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetUniformLocation.xhtml");
    return location;
}

void Shader::uploadUniformMat4(const std::string& name, const Mat4& matrix) const
{
    int location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_TRUE, (float*)&matrix);
}

void Shader::uploadUniform3f(const std::string& name, const Vec3f& vec) const
{
    int location = getUniformLocation(name);
    glUniform3f(location, vec.a[0], vec.a[1], vec.a[2]);
}

void Shader::uploadUniform4f(const std::string& name, const Vec4f& vec) const
{
    int location = getUniformLocation(name);
    glUniform4f(location, vec.a[0], vec.a[1], vec.a[2], vec.a[3]);
}

void Shader::uploadUniform1i(const std::string& name, int a) const
{
    int location = getUniformLocation(name);
    glUniform1i(location, a);
}

void Shader::uploadUniform1f(const std::string& name, float a) const
{
    int location = getUniformLocation(name);
    glUniform1f(location, a);
}