#include"entity.h"

void log(const Component& component)
{
    component.log();
}

void log(const Entity& entity)
{
    int size = entity.components.size();
    cout << "Entity " << entity.entityName << "  " << size << " components:-----" << endl;
    for(auto itComponent = entity.components.begin(); itComponent != entity.components.end(); itComponent++)
    {
        log(**itComponent);
    }
    cout << "-------------" << endl;
}

Entity::Entity()
    : entityName("nameless_entity"), components(15)
{
    //cout << "Constructor entity: " << entityName << endl;
}

Entity::Entity(const std::string& name)
    : Entity()
{
    entityName = name;
}

Entity::~Entity()
{
    //cout << "Destructor entity: " << entityName << endl;
    for(auto itComponent = components.begin(); itComponent != components.end(); itComponent++)
    {
        delete (*itComponent);
    }
}