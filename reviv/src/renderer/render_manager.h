#pragma once

#include"window.h"
#include"core/mat.h"
#include"renderer/render_command.h"
#include"renderer/texture.h"

#include"scene/components.h"
#include"scene/scene.h"

#include"renderer/material.h"

class RenderManager {
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
    // TODO: obrisi sve do private
    float renderDistance = 1000;
    
    bool renderHitbox;
    float* voxelBuffer;
    size_t sizeOfVoxelBuffer;
    bool fullscreen = false;
    
    Shader praviMono;
    Texture2D stanicTexture;
    Texture2D beloTexture;
    
    int nrOfValidCubes;
    ///

    Shader shaderTexture;
    Shader shaderMonochroma;

    Model modelCube;
    Model modelSphere;
    Model modelGun;

private:
    RenderManager() = default;

    void iInit();
    void iOnUpdate();
    void iShutdown();

    void submit(Material* pMaterial, const Mat4& transform, const Vao& vao);
    void beginScene();
    void endScene();
};