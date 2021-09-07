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
    : components(15), entityName("nameless_entity")
{
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