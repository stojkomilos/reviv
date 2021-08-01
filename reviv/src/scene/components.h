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
        //Mat4 result(1);
        Mat4 result = multiply(rotateZ(rotation.yaw), multiply(rotateY(rotation.pitch), rotateX(rotation.roll)));
        //Mat4 result = rotateZ(rotation.yaw);
        result = translate(result, Vec4f(position, 0));
        result = mat::scale(result, Vec4f(this->scale, 1));
        return result;
    }

    virtual void log() const override { cout << componentTypeName << endl; ::log(getTransform()); }
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
    virtual void log() const override { cout << componentTypeName << endl; ::log(camera); }
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
    virtual void log() const override { cout << componentTypeName << endl; ::log(model); }
};

class PointLightComponent : public SpecificComponent<PointLightComponent>
{
public:
    PointLight light;

    template<class ...Args>
    PointLightComponent(Args&&... args) : light(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("PointLightComponent"); }
    virtual void log() const override { cout << componentTypeName << endl; ::log(light); }
};

class DirectionalLightComponent : public SpecificComponent<DirectionalLightComponent>
{
public:
    DirectionalLight light;

    template<class ...Args>
    DirectionalLightComponent(Args&&... args) : light(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("DirectionalLightComponent"); }
    virtual void log() const override { cout << componentTypeName << endl; ::log(light); }
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