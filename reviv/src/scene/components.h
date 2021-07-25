#pragma once

#include"entity.h"

#include"renderer/camera.h"
#include"renderer/model.h"
#include"renderer/vertex_array.h"
#include"renderer/material.h"
#include"core/mat.h"
#include"renderer/light.h"

class TransformComponent : public SpecificComponent<TransformComponent>
{
public:
    TransformComponent() { static bool isFirstInit = runOnFirstInit("TransformComponent"); }

    Vec3f position = {0.f, 0.f, 0.f};
    Rotation rotation = Vec3f(0, 0, 0);
    Vec3f scale = {1.f, 1.f, 1.f};

    Mat4 getTransform() const
    {
        Mat4 result = identity;
        result = translate(identity, position);
        result = mat::scale(result, scale);
        //result = rotateX(result, rotation.yaw);
        return result;
    }

    void log() const override { cout << componentTypeName << endl; ::log(getTransform()); }
};

class CameraComponent : public SpecificComponent<CameraComponent>
{
public:
    Camera camera;

    template<class ...Args>
    CameraComponent(Args&&... args) : camera(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("CameraComponent"); }

    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const Camera& initCamera)
        : CameraComponent() { camera = initCamera; }

    operator const Camera& () const { return camera; }
    operator Camera& () { return camera; }
    void log() const override { cout << componentTypeName << endl; ::log(camera); }
};

class ModelComponent : public SpecificComponent<ModelComponent>
{
public:
    Model model;

    template<class ...Args>
    ModelComponent(Args&&... args) : model(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("ModelComponent"); }

    ModelComponent(const ModelComponent&) = default;

    //operator const model& () const { return model; }
    //operator model& () { return model; }
    void log() const override { cout << componentTypeName << endl; ::log(model); }
};

class PointLightComponent
{
    PointLight light;
    PointLight(Args&&... args) : light(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("PointLightComponent"); }
};

/*
template <class ComponentTemplate, class BasicTemplate>
class ComponentTemplate : public SpecificComponent<ComponentTemplate>, public <BasicTemplate>
{
public:
    <BasicTemplate> component;

    //<ComponentTemplate>() { static bool isFirstInit= runOfFirstInit("")}
    <ComponentTemplate>(const <ComponentTemplate>&) = default;
    <ComponentTemplate>(const <BasicTemplate>& initComponent)
        : <ComponentTemplate>() { component = initComponent; }

}
*/