#include"rv_pch.hpp"

#include"asset_manager.h"

void AssetManager::init()
{
    modelLoaderCube.load("assets/models/cube.fbx");
    modelLoaderSphere.load("assets/models/sphere.fbx");
    modelLoaderQuad2D.load("assets/models/quad_2d.obj");

    shaderMonochroma.init("assets/shaders/monochroma.vs", "assets/shaders/monochroma.fs");
    shaderTexture.init("assets/shaders/texture.vs", "assets/shaders/texture.fs");
    shaderPhong.init("assets/shaders/phong.vs", "assets/shaders/phong.fs");
    shaderDefferedBlinnPhong.init("assets/shaders/deffered_blinn_phong.vs", "assets/shaders/deffered_blinn_phong.fs");
    shaderDeffered.init("assets/shaders/deffered.vs", "assets/shaders/deffered.fs");

}

/*
void AssetManager::initMaterials()
{
    // Original value is 128.f.     |     32 is good pick
    float shininessConst = 128.f * 10;

    materialEmerald.setShader(&shaderPhong);
    materialRuby.setShader(&shaderPhong);
    materialGold.setShader(&shaderPhong);
    materialChrome.setShader(&shaderPhong);
    materialSilver.setShader(&shaderPhong);
    materialTurquoise.setShader(&shaderPhong);
    materialObsidian.setShader(&shaderPhong);

    materialEmerald.set("u_Material.ambient", Vec3f(0.0215f, 0.1745f, 0.0215f));
    materialEmerald.set("u_Material.diffuse", Vec3f(0.07568f, 0.61424f, 0.07568f));
    materialEmerald.set("u_Material.specular", Vec3f(0.633f, 0.727811f, 0.633f));
    materialEmerald.set("u_Material.shininess", 0.6f * shininessConst);

    materialRuby.set("u_Material.ambient", Vec3f(0.1745, 0.01175, 0.01175));
    materialRuby.set("u_Material.diffuse", Vec3f(0.61424, 0.04136, 0.04136));
    materialRuby.set("u_Material.specular", Vec3f(0.727811, 0.626959, 0.626959));
    materialRuby.set("u_Material.shininess", 0.6f * shininessConst);

    materialGold.set("u_Material.ambient", Vec3f(0.24725, 0.1995, 0.0745));
    materialGold.set("u_Material.diffuse", Vec3f(0.75164, 0.60648, 0.22648));
    materialGold.set("u_Material.specular", Vec3f(0.628281, 0.555802, 0.366065));
    materialGold.set("u_Material.shininess", 0.4f * shininessConst);

    materialChrome.set("u_Material.ambient", Vec3f(0.25, 0.25, 0.25));
    materialChrome.set("u_Material.diffuse", Vec3f(0.4, 0.4, 0.4));
    materialChrome.set("u_Material.specular", Vec3f(0.774597, 0.774597, 0.774597));
    materialChrome.set("u_Material.shininess", 0.6f * shininessConst);

    materialSilver.set("u_Material.ambient", Vec3f(0.19225, 0.19225, 0.19225));
    materialSilver.set("u_Material.diffuse", Vec3f(0.50754, 0.50754, 0.50754));
    materialSilver.set("u_Material.specular", Vec3f(0.508273, 0.508273, 0.508273));
    materialSilver.set("u_Material.shininess", 0.4f * shininessConst);

    materialTurquoise.set("u_Material.ambient", Vec3f(0.1, 0.18725, 0.1745));
    materialTurquoise.set("u_Material.diffuse", Vec3f(0.396, 0.74151, 0.69102));
    materialTurquoise.set("u_Material.specular", Vec3f(0.297254, 0.30829, 0.306678));
    materialTurquoise.set("u_Material.shininess", 0.1f * shininessConst);

    materialObsidian.set("u_Material.ambient", Vec3f(0.05375, 0.05, 0.06625));
    materialObsidian.set("u_Material.diffuse", Vec3f(0.18275, 0.17, 0.22525));
    materialObsidian.set("u_Material.specular", Vec3f(0.332741, 0.328634, 0.346435));
    materialObsidian.set("u_Material.shininess", 0.3f * shininessConst);
}
*/