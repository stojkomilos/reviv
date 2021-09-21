#pragma once

#include"entity.h"

#include"renderer/camera.h"
#include"renderer/model.h"
#include"renderer/vertex_array.h"
#include"renderer/material.h"
#include"core/mat.h"
#include"renderer/light.h"
#include"physics/physics_manager.h"
#include"physics/collision.h"

class TransformComponent : public SpecificComponent<TransformComponent>
{
public:
    TransformComponent() { static bool isFirstInit = runOnFirstInit("TransformComponent"); }

    Vec3 position = {0.f, 0.f, 0.f};
    Vec3 rotation = {0.f, 0.f, 0.f};
    Vec3 scale = {1.f, 1.f, 1.f};

    Mat<4,4> getTransform() const // potential optimization
    {
        Mat<4, 4> result = mat::scale(Vec4(scale, 1));
        result = (rotateX(rotation.get(0, 0)) * result);
        result = rotateY(rotation.get(1, 0)) * result;
        result = rotateZ(rotation.get(2, 0)) * result;
        result = mat::translate(result, Vec4(position, 0));

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

class PhysicalComponent : public SpecificComponent<PhysicalComponent>
{
public:
    PhysicalDynamic physical;

    template<class ...Args>
    PhysicalComponent(Args&&... args) : physical(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("PhysicalComponent"); }

    virtual void log() const override { cout << componentTypeName << endl; ::log(physical); }
};

class ColliderSphereComponent : public SpecificComponent<ColliderSphereComponent>
{
public:
    ColliderSphere collider;

    template<class ...Args>
    ColliderSphereComponent(Args&&... args) : collider(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("ColliderSphereComponent"); }

    virtual void log() const override { cout << componentTypeName << endl; ::log(collider); }
};

class ColliderBoxComponent : public SpecificComponent<ColliderBoxComponent>
{
public:
    ColliderBox collider;

    template<class ...Args>
    ColliderBoxComponent(Args&&... args) : collider(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("ColliderBoxComponent"); }
    virtual void log() const override { cout << componentTypeName << endl; ::log(collider); }
};

class ColliderMeshComponent : public SpecificComponent<ColliderMeshComponent>
{
public:
    ColliderMesh collider;

    template<class ...Args>
    ColliderMeshComponent(Args&&... args) : collider(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("ColliderMeshComponent"); }

    virtual void log() const override { cout << componentTypeName << endl; ::log(collider); }
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