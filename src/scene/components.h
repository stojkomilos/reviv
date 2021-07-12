#pragma once

#include"entity.h"

#include"renderer/camera.h"
#include"renderer/modelloader.h"
#include"renderer/vertex_array.h"
#include"renderer/material.h" //GLFW
#include"core/mat.h"


class PositionComponent : public SpecificComponent<PositionComponent>
{
public:
    Vec3f position;

    virtual ~PositionComponent() = default;
    PositionComponent() { static bool isFirstInit = runOnFirstInit("PositionComponent"); }
    PositionComponent(const PositionComponent&) = default;
    PositionComponent(const Vec3f& initPosition)
        : PositionComponent() { position = initPosition; }

    operator const Vec3f& () const { return position; }
    operator Vec3f& () { return position; }
    void log() const override { cout << componentTypeName << endl; ::log(position);
    }
};

class RotationComponent : public SpecificComponent<RotationComponent>
{
public:
    Rotation rotation;

    virtual ~RotationComponent() = default;
    RotationComponent() { static bool isFirstInit = runOnFirstInit("RotationComponent"); }
    RotationComponent(const RotationComponent&) = default;
    RotationComponent(const Rotation& initRotation)
        : RotationComponent() { rotation = initRotation; }

    operator const Rotation& () const { return rotation; }
    operator Rotation& () { return rotation; }
    void log() const override { cout << componentTypeName << endl; ::log(rotation); }
};

class CameraComponent : public SpecificComponent<CameraComponent>
{
public:
    Camera camera;

    CameraComponent() { static bool isFirstInit = runOnFirstInit("CameraComponent"); }
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const Camera& initCamera)
        : CameraComponent() { camera = initCamera; }

    operator const Camera& () const { return camera; }
    operator Camera& () { return camera; }
    void log() const override { cout << componentTypeName << endl; ::log(camera); }
};

class TransformComponent : public SpecificComponent<TransformComponent>
{
public:
    Mat4 transform;

    TransformComponent() { static bool isFirstInit = runOnFirstInit("TransformComponent"); }
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const Mat4& initMat4)
        : TransformComponent() { transform = initMat4; }

    operator const Mat4& () const { return transform; }
    operator Mat4& () { return transform; }
    void log() const override { cout << componentTypeName << endl; ::log(transform); }
};

class ModelLoaderComponent : public SpecificComponent<ModelLoaderComponent>
{
public:
    ModelLoader modelLoader;

    ModelLoaderComponent() { static bool isFirstInit = runOnFirstInit("ModelLoaderComponent"); }
    ModelLoaderComponent(const ModelLoaderComponent&) = default;
    ModelLoaderComponent(const ModelLoader& initModelLoader)
        : ModelLoaderComponent() { modelLoader = initModelLoader; }

    operator const ModelLoader& () const { return modelLoader; }
    operator ModelLoader& () { return modelLoader; }
    void log() const override { cout << componentTypeName << endl; ::log(modelLoader); }
};

class MaterialComponent : public SpecificComponent<MaterialComponent>
{
public:
    Material material;

    MaterialComponent() { static bool isFirstInit = runOnFirstInit("MaterialComponent"); }
    MaterialComponent(const MaterialComponent&) = default;
    MaterialComponent(const Material& initMaterial)
        : MaterialComponent() { material = initMaterial; }

    operator const Material& () const { return material; }
    operator Material& () { return material; }
    void log() const override { cout << componentTypeName << endl; ::log(material); }
};

class VaoComponent : public SpecificComponent<VaoComponent>
{
public:
    Vao vao;

    VaoComponent() { static bool isFirstInit = runOnFirstInit("VaoComponent"); }
    VaoComponent(const VaoComponent&) = default;
    VaoComponent(const Vao& initVao)
        : VaoComponent() { vao = initVao; }

    operator const Vao& () const { return vao; }
    operator Vao& () { return vao; }
    void log() const override { cout << componentTypeName << endl; ::log(vao); }
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