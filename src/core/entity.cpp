#include"entity.h"
#include"basic_components.h"
#include"renderer/material.h"
#include"renderer/camera.h"
#include"renderer/vertex_array.h"

int gid;

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

void Entity::log() const
{
    int size = components.size();
    cout << name << ".log | " << size << " Components: ";
    cout << endl;
    cout << "KURAC\n";

    for(int i=0; i<size; i++)
    {
        cout << "Ci=" << i << "C=";
        cout << components[i]->getId() << " ";
        cout << "|\n";
    }
    cout << endl;
}

//void Entity::print() const
//{
//    int size = components.size();
//    cout << name << ".log2 | " << size << " Components: ";
//
//    if(hasComponent<Material>())
//    {
//        cout << "Material " << getComponent\n";
//    }
//
//    if(hasComponent<TransformComponent>())
//    {
//        cout << "Transform \n";
//    }
//
//    if(hasComponent<RotationComponent>())
//    {
//        cout << "Rotation ";
//    }
//
//    if(hasComponent<ProjectionCamera>())
//    {
//        cout << "ProjectionCamera ";
//    }
//
//    if(hasComponent<ModelLoader>())
//    {
//        cout << "ModelLoader ";
//    }
//
//    if(hasComponent<Vao>())
//    {
//        cout << "Vao ";
//    }
//}