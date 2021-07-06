#include"entity.h"
#include"basic_components.h"
#include"renderer/material.h"
#include"renderer/camera.h"
#include"renderer/vertex_array.h"

int gid;

void Entity::log() const
{
    int size = components.size();
    cout << name << ".log | " << size << " Components: ";
    cout << endl;

    for(int i=0; i<size; i++)
    {
        //cout << "Ci=" << i << "C=";
        cout << components[i]->getId() << " ";
        //cout << "|\n";
    }
    cout << endl;
}

Component::Component()
{

}
Component::~Component()
{

}
Entity::Entity()
{
    components.clear();
}

Entity::Entity(const std::string& name)
    : name(name)
{
    components.clear();
}

//Entity::Entity(const std::string& name = "nameless")
//    : name(name)
//{
//    components.clear();
//}