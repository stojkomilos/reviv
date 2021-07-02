#include"entity.h"

int gid;

void Entity::addComponent(void* newComponent)
{
    components.push_back((Component*)newComponent);
}

//unsigned int giveId((Component*) help2)
//{
//    Component help1 = *component;
//    return help1.getId();
//}

// -------------------------------------------------------
//
//int Entity::hasComponent(std::string componentName)
//{
//    int size = components.size();
//
//    int has = 0;
//
//    for(int i=0; i<size; i++)
//    {
//        //Component currentComponent = components[i];                   TODO
//        //if(componentName == ((Component*)(components[i])).name)
//        //if(componentName == currentComponent.name)
//            has++;
//    }
//    return has;
//}
//
//RenderableComponent::RenderableComponent()
//{
//    std::cout << id << std::endl;
//    //id = 1;
//    name = "Renderable";
//}
//LightComponent::LightComponent()
//{
//    //id = 2;
//    name = "Light";
//}
//unsigned int RenderableComponent::id = RENDERABLE_ID;
//unsigned int LightComponent::id = LIGHT_ID;
