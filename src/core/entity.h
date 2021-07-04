#pragma once

#include<vector>
#include<string>
#include<iostream>
#include<assert.h>

#include"mat.h"

extern int gid;

class Component
{
public:
    std::string name;
    Vec2f position; //TODO
    virtual unsigned int getId() = 0;
    Component();
    virtual ~Component();
};

class Entity
{
public:
    Entity();
    std::vector<Component*> components;
    std::string name = "nameless";
    bool valid = true;
    void log() const;
    void print() const;

    template <class T>
    void addComponent(void* newComponent);

    template <class T>
    bool hasComponent() const;

    template <class T>
    T* getComponent() const;
};

template <class T>
class SpecificComponent : public Component
{
public:
    static unsigned int id;
    unsigned int getId() override;
    SpecificComponent();
    virtual ~SpecificComponent();
};

template <class T>
unsigned int SpecificComponent<T>::id(gid++);

template <class T>
unsigned int SpecificComponent<T>::getId()
{
    return id;
}

template <class T>
void Entity::addComponent(void* newComponent)
{
    if(((Component*)newComponent)->getId() != T::id)
    {
        cout << "ERROR: trying to add a component that does not match the type specified";
        assert(false);
    }
    cout << "==> Adding component\n";
    components.push_back((T*)newComponent);

    log();

}

template <class T>
bool Entity::hasComponent() const
{
//    log();
    int size = components.size();
    for(int i=0; i<size; i++)
    {
        cout << "D T::id = " << T::id << endl;
        if(components[i]->getId() == T::id)
        {
            return true;
        }
    }
    return false;
}

template <class T>
T* Entity::getComponent() const
{
    T* result = nullptr;

    int size = components.size();
    for(int i=0; i<size; i++)
    {
        cout << "i=" << i << " size=" << size << " T::id=" << T::id << endl;
        if((*components[i]).getId() == T::id) // ->
        {
            if(result != nullptr)
            {
                cout << "ERROR: Requested component that has multiple instances in one entity";
                assert(false);
            }
            cout << "nasao id: " << T::id << "=" << (*components[i]).getId() << endl;
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
SpecificComponent<T>::SpecificComponent()
{

}

template <class T>
SpecificComponent<T>::~SpecificComponent()
{

}