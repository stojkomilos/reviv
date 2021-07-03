#include"entity.h"

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