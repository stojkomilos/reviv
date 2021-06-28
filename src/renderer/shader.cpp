#include"shader.h"
#include"reviv/renderer/opengl_assert.h"


Shader::Shader() { 
    ID = 0;
};
void Shader::setUp(const char* vertexPath, const char* fragmentPath) { //, const char* geometryPath = nullptr)

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
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    unsigned int vertex, fragment;
    GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GLCall(glCompileShader(vertex));
    checkCompileErrors(vertex, "VERTEX");

    GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GLCall(glCompileShader(fragment));
    checkCompileErrors(fragment, "FRAGMENT");

    /*unsigned int geometry;
    if (geometryPath != nullptr) {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }*/

    GLCall(ID = glCreateProgram());
    GLCall(glAttachShader(ID, vertex));
    GLCall(glAttachShader(ID, fragment));
    //if (geometryPath != nullptr)
    //    glAttachShader(ID, geometry);
    GLCall(glLinkProgram(ID));
    checkCompileErrors(ID, "PROGRAM");

    GLCall(glDeleteShader(vertex));
    GLCall(glDeleteShader(fragment));
  //  if (geometryPath != nullptr)
  //      glDeleteShader(geometry);
}
void Shader::bind() {
    GLCall(glUseProgram(ID));
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            GLCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        GLCall(glGetProgramiv(shader, GL_LINK_STATUS, &success));
        if (!success) {
            GLCall(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::uploadUniformMat4(const std::string& name, mat::mat4& matrix)
{
    GLCall(unsigned int location = glGetUniformLocation(ID, name.c_str()));
    GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, (float*)&matrix));
}

void Shader::uploadUniform3f(const std::string& name, mat::vec3 a)
{
    GLCall(unsigned int loc = glGetUniformLocation(ID, name.c_str()));
    GLCall(glUniform3f(loc, a.x, a.y, a.z));
}

void Shader::uploadUniform4f(const std::string& name, mat::vec4 a)
{
    GLCall(unsigned int loc = glGetUniformLocation(ID, name.c_str()));
    GLCall(glUniform4f(loc, a.x, a.y, a.z, a.w));
}

void Shader::uploadUniform1i(const std::string& name, int a) {
    GLCall(unsigned int loc = glGetUniformLocation(ID, name.c_str()));
    GLCall(glUniform1i(loc, a));
}