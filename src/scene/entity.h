#pragma once

#include<vector>
#include<string>
#include<iostream>
#include<assert.h>

#include"core/mat.h"

using ComponentId = unsigned long long;

using std::cin; using std::cout; using std::endl;

class Component
{
public:
    virtual ComponentId getId() const = 0;
    virtual std::string getComponentTypeName() const = 0;
    virtual ~Component();
    Component(); // Makes it so you can't create a plain Component object, without it being a SpecificComponent (you can still create SpecificComponent without any template for an actual component)
private:
protected:
    static inline unsigned int m_GenIdCounter = 0;
};

class Entity
{
public:
    std::vector<Component*> components;
    std::string entityName;
    bool valid = true;
    Entity(const std::string& name = "nameless_entity");
    ~Entity();
    //Entity(const Entity& entity); // TODO
    void log() const;
    void print() const;

//    template <class T>
//    void add(void* newComponent);

    template <class T>
    T* add();

    template <class T>
    bool has() const;

    template <class T>
    T* get() const;
private:
};

template <class T>
class SpecificComponent : public Component
{
public:
    SpecificComponent() = default;
    virtual ~SpecificComponent();
    static ComponentId id;
    static std::string componentTypeName;
    static ComponentId generateNewComponentId(); //TODO static?
    static bool runOnFirstInit(const std::string& initCompName); //TODO static?
    ComponentId getId() const override; //TODO inline?
    std::string getComponentTypeName() const override;
protected:
    //TODO: mozda? delete na copy constructor??
    //TODO: inline da stavim na getId (nzm dal moze inline static)
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
    if(m_GenIdCounter >= 63)
    {
        cout << "ERROR: reaching the end of possible number of unique component types\n"; // possible solution: change ComponentId to be a larger unsigned type
        assert(false);
    }
    //cout << "ID: " << result << endl;

    return result;
}

//template <class T>
//ComponentId SpecificComponent<T>::id(generateNewComponentId());

template <class T>
ComponentId SpecificComponent<T>::getId() const
{
    return id;
}

template <class T>
T* Entity::add()
{
    T* result = new T;
    components.push_back(result);
    return result;
}

template <class T>
bool Entity::has() const
{
//    log();
    int size = components.size();
    for(int i=0; i<size; i++)
    {
        //cout << "D T::id = " << T::id << endl;
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
        //cout << "i=" << i << " size=" << size << " T::id=" << T::id << endl;
        Component* temp = (Component*)components[i];
        //if(((Component*)components[i]).getId() == T::id) // ->
        if(temp->getId() == T::id) // ->
        {
            if(result != nullptr)
            {
                cout << "ERROR: Requested component that has multiple instances in one entity";
                assert(false);
            }
            //cout << "nasao id: " << T::id << "=" << (*components[i]).getId() << endl;
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


template <class T>
SpecificComponent<T>::~SpecificComponent()
{

}