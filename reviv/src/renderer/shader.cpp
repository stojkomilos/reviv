#include "shader.h"

Shader::Shader()
{ 
    id = 0;
};

void Shader::init(const char* vertexPath, const char* fragmentPath)
{
    filePathVertex = vertexPath;
    filePathFragment = fragmentPath;

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

    int uniformCount;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount);

    uniformNames.reserve(uniformCount);

    glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);

    for(int i=0; i<uniformCount; i++)
    {
        GLint size;
        GLenum type;

        const GLsizei bufSize = 100; // maximum name length
        GLchar name[bufSize];

        GLsizei length = maxUniformNameLength;

        glGetActiveUniform(id, (GLuint)i, bufSize, &length, &size, &type, name);
        uniformNames.pushBack(name);
    }
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

ShaderUniformMap::~ShaderUniformMap()
{
    for(const auto& [uniformName, help] : map)
    {
        //delete help.ptr; //TODO: -Wdelete-incomplete: "warning: deleting 'void* const' is undefined" TODO:
    }
}

void ShaderUniformMap::set(const std::string& uniformName, const Vec3f& vec3f)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Vec3f(vec3f);
        foo->type = ShaderDataType::SdtFloat3;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat3, "");
        *(Vec3f*)it->second.ptr = vec3f;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, const Vec4f& vec4f)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Vec4f(vec4f);
        foo->type = ShaderDataType::SdtFloat4;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat4, "");
        *(Vec4f*)it->second.ptr = vec4f;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, int n)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new int;
        *(int*)foo->ptr = n;
        foo->type = ShaderDataType::SdtInt1;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtInt1, "");
        *(int*)it->second.ptr = n;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, float n)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new float;
        *(float*)foo->ptr = n;
        foo->type = ShaderDataType::SdtFloat1;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtFloat1, "");
        *(float*)it->second.ptr = n;
    }
}

void ShaderUniformMap::set(const std::string& uniformName, double n)
{
    RV_ASSERT(false, "seting doubles is disabled (for safety)");
}

void log(const ShaderUniformMap& shaderUniformMap)
{
    cout << "Uniforms:----" << endl;
    for(const auto& [key, value] : shaderUniformMap.map)
    {
        logSpecificUniform(shaderUniformMap, key);
    }

    cout << "Textures:----" << endl;
    for(const auto& [key, value] : shaderUniformMap.textureMap)
    {
        cout << key << " : " << value << endl;
    }
}

void logSpecificUniform(const ShaderUniformMap& shaderUniformMap, const std::string& uniformName)
{
    auto fromMap = shaderUniformMap.map.find(uniformName);
    if(fromMap == shaderUniformMap.map.end())
    {
        cout << uniformName << ": UNIFORM NOT DEFINED" << endl;
        RV_ASSERT(false, "");
        return;
    }
    const ShaderUniformHelpingStruct* value = &fromMap->second;

    cout << uniformName << ": " << endl;
    switch(value->type)
    {
        case ShaderDataType::SdtFloat1:           log(*(Vec1f*)value->ptr);      break;
        case ShaderDataType::SdtFloat2:          log(*(Vec2f*)value->ptr);      break;
        case ShaderDataType::SdtFloat3:          log(*(Vec3f*)value->ptr);     break;
        case ShaderDataType::SdtFloat4:         log(*(Vec4f*)value->ptr);     break;
        case ShaderDataType::SdtMat3:            log(*(Mat3*)value->ptr);       break;
        case ShaderDataType::SdtMat4:            log(*(Mat4*)value->ptr);       break;
        case ShaderDataType::SdtInt1:             log(*(Vec1i*)value->ptr);       break;
        case ShaderDataType::SdtInt2:            log(*(Vec2i*)value->ptr);       break;
        case ShaderDataType::SdtInt3:            log(*(Vec3i*)value->ptr);       break;
        case ShaderDataType::SdtInt4:            log(*(Vec4i*)value->ptr);       break;
        case ShaderDataType::SdtBool:            log(*(bool*)value->ptr);       break;
        default: RV_ASSERT(false, "ERROR: ShaderUniformHelpingStruct data type not defined in log function");
    }
    cout << endl;
}

void ShaderUniformMap::uploadUniform(const Shader& shader, const std::string& existingUniformName) const
{
    shader.bind();

    ShaderUniformHelpingStruct help;

    auto iterator = map.find(existingUniformName);
    RV_ASSERT(iterator != map.end(), "uniform does not exist in ShaderUniformMap already");

    help.ptr = iterator->second.ptr;
    help.type = iterator->second.type;

    //cout << "MATERIAL=============" << endl;
    switch(help.type)
    {
        case ShaderDataType::SdtMat4:
            shader.uploadUniformMat4(existingUniformName, *(Mat4*)(help.ptr));
            //cout << "material uniform: " << uniformName;
            //log(*(Mat4*)(help.ptr));
            break;

        case ShaderDataType::SdtFloat1:
            shader.uploadUniform1f(existingUniformName, *(float*)(help.ptr));
            break;

        case ShaderDataType::SdtFloat3:
            shader.uploadUniform3f(existingUniformName, *(Vec3f*)(help.ptr));
            break;

        case ShaderDataType::SdtFloat4:
            shader.uploadUniform4f(existingUniformName, *(Vec4f*)(help.ptr));
            break;

        case ShaderDataType::SdtInt1:
            shader.uploadUniform1i(existingUniformName, *(int*)(help.ptr));
            break;

        default:
            RV_ASSERT(false, "ERROR: Specified uniform data type not found");
    }
}

void ShaderUniformMap::uploadAllUniforms(const Shader& shader) const
{
    shader.bind();

    ShaderUniformHelpingStruct help;
    for(const auto& [uniformName, help] : map)
    {
        uploadUniform(shader, uniformName);
    }
}

void ShaderUniformMap::set(const std::string& uniformName, const Mat4& mat4)
{
    auto it = map.find(uniformName);
    if(it == map.end())
    {
        ShaderUniformHelpingStruct* foo = &map[uniformName];
        foo->ptr = new Mat4(mat4);
        foo->type = ShaderDataType::SdtMat4;
    }
    else {
        RV_ASSERT(it->second.type == ShaderDataType::SdtMat4, "");
        *(Mat4*)it->second.ptr = mat4;
    }
}

void ShaderUniformMap::setTexture(const std::string& textureUniformName, const Texture& texture)
{
    textureMap[textureUniformName] = &texture;
}
