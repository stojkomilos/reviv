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
    mat::vec2 position; //TODO
    virtual unsigned int getId() = 0;
    Component()
    {

    }
    virtual ~Component()
    {

    }
};


class Entity
{
public:
    std::vector<Component*> components;

    template <class T>
    T* getComponent();

    void addComponent(void* newComponent);
};

template <class T>
class SpecificComponent : public Component
{
public:
    static unsigned int id;
    T* getComponent();
    unsigned int getId() override;
    SpecificComponent()
    {

    }
    virtual ~SpecificComponent()
    {

    }
};

template <class T>
unsigned int SpecificComponent<T>::id(gid++);

//class kurac
//{
//public:
//    unsigned int giveId(int help2);
//};

template <class T>
unsigned int SpecificComponent<T>::getId()
{
    return id;
}

template <class T>
T* Entity::getComponent()
{
    int size = components.size();
    for(int i=0; i<size; i++)
    {
        if((*components[i]).getId() == T::id) // ->
        {
            std::cout << "nasao id: " << T::id << "=" << (*components[i]).getId() << std::endl;
            return  (T*)components[i];
        }
    }
    std::cout << "ERROR, nije nasao, entity.h, L:54\n" << std::endl;
    assert(false);
    return nullptr;
}


//template <class T>
//unsigned int SpecificComponent<T>::getId()
//{
//    return id;
//}

//template <class T>
//SpecificComponent<T>::SpecificComponent()
//{
//SpecificComponent<T>::id = gid++;
//std::cout << "konstruktor id: " << id << " gid: " << gid << std::endl;
//}
