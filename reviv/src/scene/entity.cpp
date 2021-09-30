#include"entity.h"
#include"components.h"

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

// ------ component specialization

RigidbodyComponent* Entity::addRigidbodyComponent(const RigidbodyShape& rigidbodyShape, const ColliderShape& colliderShape)
{
    Component* result = new RigidbodyComponent(rigidbodyShape, get<TransformComponent>());
    components.pushBack(result);

    Rigidbody* pRigidbody = &((RigidbodyComponent*)result)->rigidbody;

    switch (colliderShape)
    {
        case ColliderShape::SPHERE:
            pRigidbody->setColliderSphere();
            break;
        case ColliderShape::MESH:
            pRigidbody->setColliderMesh(get<ModelComponent>()->model.pMeshes[0]);
            if(get<ModelComponent>()->model.pMeshes[0]->m_Indices.size() > 1000)
            {
                cout << "WARNING: large mesh used potentialy in collision detection. Potential slowdown" << endl;
            }
            break;
        default:
            RV_ASSERT(false, ""); // should not be in here
    }

    return (RigidbodyComponent*)result;
}