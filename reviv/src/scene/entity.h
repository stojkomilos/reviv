#pragma once

#include"rv_pch.hpp"

#include"stls/stable_vector.h"
#include"core/mat.h"

using std::cin; using std::cout; using std::endl;

using ComponentId = unsigned long long;

class Entity;

class Component
{
public:
    virtual ~Component() = default;
    Component(const Component&) = default;
    Component& operator=(const Component&) = default;

    virtual ComponentId getId() const = 0;
    virtual std::string getComponentTypeName() const = 0;
    virtual void log() const = 0;

    inline Entity* getEntity() { return pEntity; } // returns entity that owns the component

protected:
    Component() = default;
    static inline unsigned int m_GenIdCounter = 0;

private:
    Entity* pEntity;
    friend class Entity;
};

class RigidbodyComponent; // all 3 are used for addRigidbodyComponent
enum class RigidbodyShape;
enum class ColliderShape;

class Entity
{
public:
    Entity();
    Entity(const std::string& name);
    ~Entity();

    Entity(const Entity&) = delete; // can be implemented
    Entity& operator=(const Entity&) = delete; // can be implemented

    //RigidbodyComponent* add(RigidbodyShape rigidbodyShape, ColliderShape colliderShape);

    template <class T, class... Args>
    T* add(Args&&... args);

    RigidbodyComponent* addRigidbodyComponent(const RigidbodyShape& rigidbodyShape, const ColliderShape& colliderShape); // TODO: remove after making custom DOD ecs

    template <class T>
    bool has() const;

    template <class T>
    T* get() const;

    stls::StableVector<Component*> components;
    std::string entityName;
    bool valid = true;
private:
};

void log(const Component& component);
void log(const Entity& entity);

template <class T>
class SpecificComponent : public Component
{
public:
    virtual ~SpecificComponent() = default;
    static ComponentId id;
    static std::string componentTypeName;
    inline ComponentId getId() const override;
    std::string getComponentTypeName() const override;
protected:
    SpecificComponent() = default;
    static bool runOnFirstInit(const std::string& initCompName);
private:
    static ComponentId generateNewComponentId();
};

template <class T>
ComponentId SpecificComponent<T>::id(0);

template <class T>
std::string SpecificComponent<T>::componentTypeName(std::string("ERROR: unnamed component type"));

template <class T>
bool SpecificComponent<T>::runOnFirstInit(const std::string& initCompName)
{
    id = generateNewComponentId();
    componentTypeName = initCompName;
    return true;
}

template <class T>
ComponentId SpecificComponent<T>::generateNewComponentId()
{
    ComponentId result = (ComponentId) 1 << m_GenIdCounter;
    m_GenIdCounter++;
    RV_ASSERT(m_GenIdCounter < 63, "reaching the end of possible number of unique component types\n | possible solution: change ComponentId to be a larger unsigned type");

    return result;
}

template <class T>
ComponentId SpecificComponent<T>::getId() const
{
    return id;
}

template <class T>
bool Entity::has() const
{
    int size = components.size();
    for(int i=0; i<size; i++)
    {
        if(components[i]->getId() == T::id)
        {
            return true;
        }
    }
    return false;
}

template <class T, class... Args>
T* Entity::add(Args&&... args)
{
    RV_ASSERT(has<T>() == false, "One entity can't have more than 1 of the same component type");

    Component* result = new T(std::forward<Args>(args)...);
    result->pEntity = this;
    components.pushBack(result);

    return (T*)result;
}

template <class T>
T* Entity::get() const
{
    T* result = nullptr;

    int size = components.size();
    for(int i=0; i<size; i++)
    {
        Component* temp = (Component*)components[i];
        if(temp->getId() == T::id)
        {
            RV_ASSERT(result == nullptr, "Requested component that has multiple instances in one entity");
            result = (T*)components[i];
        }
    }
    RV_ASSERT(result != nullptr, "component not found");

    return result;
}

template <class T>
std::string SpecificComponent<T>::getComponentTypeName() const
{
    return componentTypeName;
}


// ------ component specialization ---------
// (should be last line in this file)

#include"component_specialization.h"