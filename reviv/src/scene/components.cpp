#include"components.h"

#include"entity.h"

void TransformComponent::setScale(const Vec3& newScale)
{
    scale = newScale;

    if(getEntity()->has<RigidbodyComponent>())
        getEntity()->get<RigidbodyComponent>()->rigidbody.calculateMomentOfInertia();
}
