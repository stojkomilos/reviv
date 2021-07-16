#pragma once

#include"window.h"
#include"core/mat.h"
#include"renderer/render_command.h"
#include"renderer/texture.h"

#include"scene/components.h"
#include"scene/scene.h"

#include"renderer/material.h"
#include"renderer/window.h"

class RenderManager {
public:
    ~RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;
    
    static void init(const WindowData& windowData) { getInstance()->iInit(windowData); }
    static void onUpdate(const WindowData& windowData) { getInstance()->iOnUpdate(windowData); }
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
private:
    RenderManager() = default;

    void iInit(const WindowData& windowData);
    void iOnUpdate(const WindowData& windowData);
    void iShutdown();

    void submit(Material* pMaterial, const Mat4& transform, const Vao& vao);
    void beginScene(const WindowData& windowData);
    void endScene();
};