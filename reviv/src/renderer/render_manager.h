#pragma once

#include"framebuffer.h"
#include"skybox.h"
#include"renderer/deffered.h"
#include"environment.h"

#include"game_stuff/weather.h"

class RenderManager
{
public:
    ~RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;
    
    static RenderManager* get()
    {
        static RenderManager instance;
        return &instance;
    }

    Skybox skybox;
    Shader shaderDefferedGeometry; // currently deffered geometry shader
    Shader directionalShadowMapShader;
    Shader omnidirectionalShadowMapShader;
    Shader shaderDefferedLighting;
    Shader shaderMonochroma;
    Shader shaderBlend;

    //Framebuffer screenFramebuffer;
    Framebuffer defaultFramebuffer;
    //Shader screenShader;
    //Shader depthTestShader;
    
    Material materialDefferedLighting; // special material, bind sort of environment stuff actually

    Deffered deffered;

    void bindEnvironmentAndMaterial(Shader* shader, Environment* environment, Material* material);
    void bindEnvironment(Shader* shader, Environment* environment);
    void bindMaterial(Shader* shader, Material* material);

    void onEvent(Event* event);

    void init();
    void onUpdate();
    void shutdown();

    struct EntityDistance
    {
        Entity* pEntity;
        float distance;
    };

private:
    RenderManager() = default;

    void renderSceneToFramebuffer(Framebuffer* pFramebuffer);

    void shadowMapRenderPass();
    void defferedGeometryRenderPass();
    void defferedLightingRenderPass(Framebuffer *pFramebuffer);
    void forwardMonochromaRenderPass(Framebuffer *pFramebuffer);
    void forwardBlendRenderPass(Framebuffer *pFramebuffer);

    void copyDefferedDepthToFramebuffer(Framebuffer* pFramebuffer);

    void beginScene();


    Environment environment;

    void sortTransparentObjectsByDistance();
    stls::StableVector<EntityDistance> transparentEntityList; // used to render the transparent models by their distance
};
