#pragma once

#include"renderer/model_loader.h"
#include"renderer/shader.h"
#include"renderer/material.h"

class AssetManager
{
public:
    static AssetManager* get()
    {
        static AssetManager instance;
        return &instance;
    }
    void init();
    void initMaterials();

    ModelLoader modelLoaderCube;
    ModelLoader modelLoaderSphere;
    ModelLoader modelLoaderQuad2D;

    //Mesh meshQuad2D;

    Shader shaderPhong;
    Shader shaderMonochroma;
    Shader shaderTexture;

    // Material data available here: http://devernay.free.fr/cours/opengl/materials.html
    Material materialEmerald,
                materialRuby,
                materialGold,
                materialChrome,
                materialSilver,
                materialTurquoise,
                materialObsidian;
private:
    AssetManager() = default;
};