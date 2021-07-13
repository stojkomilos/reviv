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
    RenderManager() = default;
    ~RenderManager() = default;
    
    void init();
    void render();
    void shutdown();
    
    void submit(Material* material, const Mat4& transform, const Vao& vao);
    
    void beginScene();
    void endScene();

    Window window;
    
    float renderDistance = 1000;
    
    bool renderHitbox;
    float* voxelBuffer;
    size_t sizeOfVoxelBuffer;
    bool fullscreen = false;
    
    Shader praviMono;
    Texture2D stanicTexture;
    Texture2D beloTexture;
    
    int nrOfValidCubes;

private:
};