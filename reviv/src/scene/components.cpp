#include"components.h"

void TransformComponent::setScale(const Vec3& newScale)
{
    scale = newScale;
    //if(getEntity()->has<ColliderComponent>() && getEntity()->has<PhysicalComponent>())
    if(getEntity()->has<PhysicalComponent>())
    {
        //get<PhysicalComponent>()->update(TransformComponent* this, );
    }
}