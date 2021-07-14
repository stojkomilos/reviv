#pragma once

#include"entity.h"

#include"renderer/camera.h"
#include"renderer/model.h"
#include"renderer/vertex_array.h"
#include"renderer/material.h" //GLFW
#include"core/mat.h"


class PositionComponent : public SpecificComponent<PositionComponent>
{
public:
    Vec3f position;

    template<class ...Args>
    PositionComponent(Args&&... args) : position(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("PositionComponent"); }

    virtual ~PositionComponent() = default;
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

    template<class ...Args>
    RotationComponent(Args&&... args) : rotation(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("RotationComponent"); }

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

    template<class ...Args>
    CameraComponent(Args&&... args) : camera(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("CameraComponent"); }

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

    template<class ...Args>
    TransformComponent(Args&&... args) : transform(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("TransformComponent"); }

    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const Mat4& initMat4)
        : TransformComponent() { transform = initMat4; }

    operator const Mat4& () const { return transform; }
    operator Mat4& () { return transform; }
    void log() const override { cout << componentTypeName << endl; ::log(transform); }
};

class ModelComponent : public SpecificComponent<ModelComponent>
{
public:
    Model Model;

    template<class ...Args>
    ModelComponent(Args&&... args) : Model(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("ModelComponent"); }

    ModelComponent(const ModelComponent&) = default;
    ModelComponent(const Model& initModel)
        : ModelComponent() { Model = initModel; }

    operator const Model& () const { return Model; }
    operator Model& () { return Model; }
    void log() const override { cout << componentTypeName << endl; ::log(Model); }
};

class MaterialComponent : public SpecificComponent<MaterialComponent>
{
public:
    Material material;

    template<class ...Args>
    MaterialComponent(Args&&... args) : material(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("ModelComponent"); }

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

    template<class ...Args>
    VaoComponent(Args&&... args) : vao(std::forward<Args>(args)...) { static bool isFirstInit = runOnFirstInit("ModelComponent"); }

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