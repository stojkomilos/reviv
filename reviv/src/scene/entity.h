#pragma once

#include<vector>
#include<string>
#include<iostream>
#include<assert.h>

#include"stls/stable_vector.h"
#include"core/mat.h"

using std::cin; using std::cout; using std::endl;

using ComponentId = unsigned long long;

class Component
{
public:
    virtual ~Component() = default;
    virtual ComponentId getId() const = 0;
    virtual std::string getComponentTypeName() const = 0;
    virtual void log() const = 0;
protected:
    Component() = default;
    static inline unsigned int m_GenIdCounter = 0;
};

class Entity
{
public:
    Entity();
    Entity(const std::string& name);
    ~Entity();
    void print() const;

    template <class T, class... Args>
    T* add(Args&&... args);

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
    static ComponentId generateNewComponentId();
    static bool runOnFirstInit(const std::string& initCompName);
    inline ComponentId getId() const override;
    std::string getComponentTypeName() const override;
protected:
    SpecificComponent() = default;
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
    ComponentId result = 1 << m_GenIdCounter;
    m_GenIdCounter++;
    assert(m_GenIdCounter < 63); // ERROR: reaching the end of possible number of unique component types\n | possible solution: change ComponentId to be a larger unsigned type

    return result;
}

template <class T>
ComponentId SpecificComponent<T>::getId() const
{
    return id;
}

template <class T, class... Args>
T* Entity::add(Args&&... args)
{
    Component* result = new T(std::forward<Args>(args)...);
    components.pushBack(result);
    return (T*)result;
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
            assert(result == nullptr); // ERROR: Requested component that has multiple instances in one entity
            result = (T*)components[i];
        }
    }

    if(result == nullptr)
    {
        cout << "ERROR, nije nasao" << endl;
        assert(false);
    }

    return result;
}

template <class T>
std::string SpecificComponent<T>::getComponentTypeName() const
{
    return componentTypeName;
}