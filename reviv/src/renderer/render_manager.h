#pragma once

#include"core/mat.h"
#include"renderer/render_command.h"
#include"scene/scene.h"
#include"scene/asset_manager.h"
#include"framebuffer.h"
#include"skybox.h"
#include"core/time.h"
#include"core/application.h"
#include"renderer/deffered.h"
#include"environment.h"


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

    Skybox skybox;
    Shader shadowMapShader;

    //Framebuffer screenFramebuffer;
    //Shader screenShader;
    //Shader depthTestShader;

    Shader shaderDefferedBlinnPhong;
    Material materialDefferedBlinnPhong; // special material, bind sort of environment stuff actually
    Deffered deffered;

    void bindEnvironmentAndMaterial(Shader* shader, Environment* environment, Material* material);
    void bindEnvironment(Shader* shader, Environment* environment);

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