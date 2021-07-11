#include"entity.h"
#include"components.h"

void Entity::log() const
{
    int size = components.size();
    //cout << entityName << ".log | " << size << " Components: ";
    //cout << endl;

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
    cout << "DESTRUKT: EntName: " << entityName << " compSize=" << components.size() << endl;
    cout << "a\n";
    //for (int i=0; i<components.size(); i++)
    //for (Component* component : components)
    {
        //delete components[i];
    }
}