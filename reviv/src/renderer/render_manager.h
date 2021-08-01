#pragma once

#include"core/mat.h"
#include"renderer/render_command.h"
#include"scene/scene.h"
#include"scene/asset_manager.h"
#include"framebuffer.h"
#include"skybox.h"
#include"core/time.h"
#include"core/application.h"

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
    Framebuffer screenFramebuffer;
    Shader screenShader;
    ShadowMap shadowMap;

    Shader depthTestShader;

    Shader shadowMapShader;

private:
    RenderManager() = default;

    void iInit();
    void iOnUpdate();
    void iShutdown();

    void submit(const Model& model, const Mat4& transform);

    void beginScene();
    void endScene();
    void bindEnvironment(const Shader& shader, const Mat4& transform);
};