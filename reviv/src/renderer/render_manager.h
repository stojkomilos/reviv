#pragma once

#include"core/mat.h"
#include"renderer/render_command.h"
#include"scene/scene.h"
#include"scene/asset_manager.h"
#include"framebuffer.h"
#include"skybox.h"
#include"core/time.h"
#include"core/application.h"

class RenderManager;

class Environment
{
public:
    ShaderUniformMap shaderUniformMap;

    void bind(const Shader& shader) const;

    void setLights();

    inline void set(const std::string& uniformName, const Mat4& mat4) { shaderUniformMap.set(uniformName, mat4); }
    inline void set(const std::string& uniformName, const Vec4f& vec4f) { shaderUniformMap.set(uniformName, vec4f); }
    inline void set(const std::string& uniformName, const Vec3f& vec3f) { shaderUniformMap.set(uniformName, vec3f); }
    inline void set(const std::string& uniformName, int n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, float n) { shaderUniformMap.set(uniformName, n); }
    inline void set(const std::string& uniformName, double n) { shaderUniformMap.set(uniformName, n); }
private:
    Environment() = default;
    friend class RenderManager;
};

class RenderManager
{
public:
    ~RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;
    
    static void init() { getInstance()->iInit(); }
    static void onUpdate() { getInstance()->iOnUpdate(); }
    static void shutdown() { getInstance()->iShutdown(); }
    
    static RenderManager* getInstance()
    {
        static RenderManager instance;
        return &instance;
    }

    //Skybox skybox;
    //Framebuffer screenFramebuffer;
    //Shader screenShader;
    //ShadowMap shadowMap;

    //Shader depthTestShader;
    //Shader shadowMapShader;

    Shader defferedShader;
    Framebuffer gBuffer;
    Texture2D gPosition, gNormal, gAlbedoSpecular;

private:
    RenderManager() = default;

    void iInit();
    void iOnUpdate();
    void iShutdown();


    void beginScene();
    //void bindEnvironment(const Shader& shader, const Mat4& transform);
    void bindEnvironment(const Shader& shader);

    //void endScene();
    //void submit(const Model& model, const Mat4& transform);
    Environment environment;
};
