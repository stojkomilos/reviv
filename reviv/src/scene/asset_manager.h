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

    ModelLoader modelLoaderCube;
    ModelLoader modelLoaderSphere;
    ModelLoader modelLoaderQuad2D;

    ModelLoader modelLoaderDodik;
    ModelLoader modelLoaderHexagon;
    ModelLoader modelLoaderStar;
    ModelLoader modelLoaderCylinder;
    ModelLoader modelLoaderTorus;

    // Material data available here: http://devernay.free.fr/cours/opengl/materials.html
    //Material materialEmerald,
    //            materialRuby,
    //            materialGold,
    //            materialChrome,
    //            materialSilver,
    //            materialTurquoise,
    //            materialObsidian;

    //void initMaterials();
private:
    AssetManager() = default;
};