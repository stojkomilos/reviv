#include"entity.h"
#include"components.h"

void Entity::log() const
{
    int size = components.size();
    cout << entityName << ".log | " << size << " Components: ";
    cout << endl;

    for(int i=0; i<size; i++)
    {
        cout << components[i]->getId() << " ";
    }
    cout << endl;
}

Component::Component()
{

}
Component::~Component()
{

}
//Entity::Entity()
//{
//    components.clear();
//}

Entity::Entity(const std::string& name)
{
    cout << "Entity constructor: name: " << name << " nrCom: " << components.size() << endl;
    entityName = name;
    components.clear();
}

Entity::~Entity()
{
    for (int i=0; i<components.size(); i++)
    //for (Component* component : components)
    {
        cout << "DESTRUKT: EntName: " << entityName << " i=" << i << " compSize=" << components.size() << endl;
        //delete components[i];
    }
}
//Entity::Entity(const std::string& name = "nameless")
//    : name(name)
//{
//    components.clear();
//}