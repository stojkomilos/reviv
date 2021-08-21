#pragma once

#include"core/mat.h"
#include"renderer/render_command.h"
#include"scene/scene.h"
#include"core/time.h"
#include"core/application.h"
#include"shader_uniform_map.h"

class Environment
{
public:
    ShaderUniformMap shaderUniformMap;

    void bind(Shader* shader);

    void setLights();
    void setShadowMap(Entity* pEntity, Light* pLight, const std::string& uniformNameLight);

    inline void set(const std::string& uniformName, const Mat4& mat4) { shaderUniformMap.set(uniformName, mat4); }
    inline void set(const std::string& uniformName, const Vec4f& vec4f) { shaderUniformMap.set(uniformName, vec4f); }
    inline void set(const std::string& uniformName, const Vec3f& vec3f) { shaderUniformMap.set(uniformName, vec3f); }
    inline void set(const std::string& uniformName, int n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, float n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, double n) { shaderUniformMap.set(uniformName, n); }
    //inline void set(const std::string& uniformName, unsigned int n) { shaderUniformMap.set(uniformName, n); }

    inline void setTexture(const std::string& textureUniformName, const Texture& texture) { shaderUniformMap.setTexture(textureUniformName, texture); }

    int nrPointLights = 0;
    int nrDirectionalLights = 0;

private:
    Environment() = default;
    friend class RenderManager;
};

void log(const Environment& environment);