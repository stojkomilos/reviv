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

    void setLights();
    void setShadowMap(Entity* pEntity, Light* pLight, const std::string& uniformNameLight);

    inline void set(const std::string& uniformName, const Mat<4,4>& mat4) { shaderUniformMap.set(uniformName, mat4); }
    inline void set(const std::string& uniformName, const Vec4& vec4) { shaderUniformMap.set(uniformName, vec4); }
    inline void set(const std::string& uniformName, const Vec3& vec3) { shaderUniformMap.set(uniformName, vec3); }
    inline void set(const std::string& uniformName, int n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, float n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, double n) { shaderUniformMap.set(uniformName, n); }
    //inline void set(const std::string& uniformName, unsigned int n) { shaderUniformMap.set(uniformName, n); }

    inline void setTexture(const std::string& textureUniformName, const Texture& texture) { shaderUniformMap.setTexture(textureUniformName, texture); }

    int nrPointLights = 0;
    int nrDirectionalLights = 0;

private:
    Environment() = default;
    void bind(Shader* shader); // NOTE: don't call bind() on it's own. You MUST use the RenderManager wrappers
    friend class RenderManager;
};

void log(const Environment& environment);