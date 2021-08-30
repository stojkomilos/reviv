#include"collision_manager.h"

#include<cfloat>

#include"physics_manager.h"
#include"scene/components.h"

Vec3f CollisionManager::doSupportFunction(const Vec3f& direction, Collider* pFirstCollider, TransformComponent* pFirstTransform, Collider* pSecondCollider, TransformComponent* pSecondTransform)
{
    return pFirstCollider->findFurthestPoint(direction, pFirstTransform) - pSecondCollider->findFurthestPoint(-direction, pSecondTransform);
}

CollisionPoints CollisionManager::gjk(Collider* pFirstCollider, TransformComponent* pFirstTransform, Collider* pSecondCollider, TransformComponent* pSecondTransform) 
{
    if(module(pFirstTransform->position - pSecondTransform->position) < 1.9) // temp
    {
        cout << "SHOULD BE COLLIDING";
        cout << endl;
    }

    // TODO: template this so that it does not use dynamic dispatch every time. also requires templating every time you call gjk, which is fine

    Vec3f direction = pSecondTransform->position - pFirstTransform->position;

    Vec3f point1 = doSupportFunction(direction, pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);

    SimplexHelpingStruct simplex;
    simplex.points[0] = point1;
    simplex.size = 1;

    CollisionPoints collisionPoints;
    collisionPoints.hasCollided = false;

    if(dot(direction, simplex.points[0]) <= 0)
    {
        cout << "reseno na startu" << endl;
        return collisionPoints;
    }
    else // temp
    {
        cout << endl;
        //RV_ASSERT(false, "");
    }

    direction = -simplex.points[0];
    //direction = -direction;

    while(true)
    {
        Vec3f newPoint = doSupportFunction(direction, pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);
        if(dot(direction, newPoint) <= 0.0) // mozda < umesto <=
        {
            collisionPoints.hasCollided = false;
            break;
        }

        if(simplex.size == 4)
        {
            simplex.points[0] = simplex.points[1];
            simplex.points[1] = simplex.points[2];
            simplex.points[2] = simplex.points[3];

            simplex.points[3] = newPoint;
        }
        else {
            RV_ASSERT(simplex.size >= 0 && simplex.size <= 3, "unrecognized simplex size");
            simplex.points[simplex.size] = newPoint;
            simplex.size++;
        }

        if(gjkHandleSimplex(&simplex, &direction))
        {
            collisionPoints.hasCollided = true;
            cout << "breakPls?" << endl;
            break;
        }
    }

    return collisionPoints;
}

// checks if 2 vectors are pointing in the same direction / same half-plane
inline static bool sDir(const Vec3f& first, const Vec3f& second) { return dot(first, second) > 0; } 

static bool gjkHandlePoint(SimplexHelpingStruct* pSimplex, Vec3f* pDirection)
{
    RV_ASSERT(pSimplex->size == 1, "invalid simplex size");
    *pDirection = -pSimplex->points[0];
    return false;
}

static bool gjkHandleLine(SimplexHelpingStruct* pSimplex, Vec3f* pDirection)
{
    RV_ASSERT(pSimplex->size == 2, "invalid simplex size");

    Vec3f a = pSimplex->points[0];
    Vec3f b = pSimplex->points[1];
    Vec3f ab = b - a;
    Vec3f ao = -a;

    RV_ASSERT(pSimplex->size == 2, "invalid simplex size");

    if(checkIfPointBelongsToLine(a, b, Vec3f(0, 0, 0))) //TODO: mozda temp, testiraj puno
    {
        RV_ASSERT(false, ""); // temp, if breaks here -> keep it maybe?
        return true;
    }
        
    if(sDir(ab, ao))
    {
        *pDirection = cross(cross(ab, ao), ab);
    }
    else {
        pSimplex->points[0] = pSimplex->points[1];
        pSimplex->size--;
        gjkHandlePoint(pSimplex, pDirection);
    }

    return false;
}

static bool gjkHandleTriangle(SimplexHelpingStruct* pSimplex, Vec3f* pDirection)
{
    RV_ASSERT(pSimplex->size == 3, "invalid simplex size");

    Vec3f a = pSimplex->points[0];
    Vec3f b = pSimplex->points[1];
    Vec3f c = pSimplex->points[2];

    Vec3f ab = b - a;
    Vec3f ac = c - a;

    Vec3f bc = c - b;

    Vec3f ao = -a;
    Vec3f bo = -b;
    Vec3f co = -c;

    Vec3f abc = cross(ab, ac);

    Vec3f acPerp = cross(abc, ac);
    Vec3f bcPerp = cross(abc, -bc);

    if(sDir(abc, cross(ab, ao)) && sDir(abc, cross(bc, bo)) && sDir(abc, cross(-ac, co))) // temp
    {
        cout << "kao nasao u 2 dimenzije?" << endl;
    }
    if(sDir(acPerp, ao))
    {
        if(sDir(-ac, co))
        {
            pSimplex->size = 2;
            pSimplex->points[1] = pSimplex->points[2];
            //gjkHandleLine(pSimplex, pDirection);
        }
        else {   
            pSimplex->size = 2;
            pSimplex->points[1] = pSimplex->points[2];
            gjkHandleLine(pSimplex, pDirection);
        }
        return false;
    }
    else if(sDir(bcPerp, bo))
    {
        if(sDir(-bc, co))
        {
            pSimplex->size = 2;
            pSimplex->points[0] = pSimplex->points[1];
            pSimplex->points[1] = pSimplex->points[2];
            //return gjkHandleLine(pSimplex, pDirection);
            //gjkHandleLine(pSimplex, pDirection);
        }
        else {
            pSimplex->points[0] = pSimplex->points[1];
            pSimplex->points[1] = pSimplex->points[2];
            pSimplex->size = 2;
            gjkHandleLine(pSimplex, pDirection);
        }
        return false;
    }
    else { // is "inside" the triangle projection
        if(sDir(ao, abc)) // is "above"
        {
            *pDirection = abc;
            RV_ASSERT(sDir(*pDirection, ao), "");
        }
        else { // is "below"
            *pDirection = -abc;
            pSimplex->points[0] = b;
            pSimplex->points[1] = a;
            pSimplex->points[2] = c;
            //RV_ASSERT(sDir(*pDirection, ao), "");
        }
        cout << "HAHA!" << endl;
        return false;
    }
}

static bool gjkHandleTetrahedron(SimplexHelpingStruct *pSimplex, Vec3f* pDirection)
{
    Vec3f a = pSimplex->points[0];
    Vec3f b = pSimplex->points[1];
    Vec3f c = pSimplex->points[2];
    Vec3f d = pSimplex->points[3];

    Vec3f ab = b - a;
    Vec3f ac = c - a;
    Vec3f ad = d - a;

    Vec3f bc = c - b;
    Vec3f bd = d - b;

    Vec3f cd = d - c;

    Vec3f abc = cross(ab, ac); // treba samo kod triangle dela

    Vec3f acPerp = cross(abc, ac);
    Vec3f bcPerp = cross(abc, -bc);

    Vec3f ao = -a;
    Vec3f bo = -b;
    Vec3f co = -c;
    Vec3f doo = -d;


    Vec3f abd = cross(ab, ad);
    Vec3f acd = cross(ad, ac);
    Vec3f bcd = cross(bc, bd);

    if(module(a - b) < 0.0001f || module(a - c) < 0.0001f || module(a - d) < 0.0001f || module(b - c) < 0.0001f || module(b - d) < 0.0001f || module(c - d) < 0.0001f)
    {
        cout << endl;
    }


    cout << "a: ";
    log(a);

    cout << "b: ";
    log(b);

    cout << "c: ";
    log(c);

    cout << "d: ";
    log(d);
    cout << endl << endl;

    RV_ASSERT(sDir(*pDirection, ao), "invalid origin position");

    //RV_ASSERT(!sDir(acPerp, ao), ""); // all 3 temp
    //RV_ASSERT(!sDir(bcPerp, bo), "");
    //RV_ASSERT((sDir(abc, cross(ab, ao)) && sDir(abc, cross(bc, bo)) && sDir(abc, cross(-ac, co))), "");


    if(sDir(abd, ao))
    {
        pSimplex->points[2] = pSimplex->points[3];
        pSimplex->size--;
        gjkHandleTriangle(pSimplex, pDirection);
        return false;
    }

    if(sDir(acd, ao))
    {
        pSimplex->points[1] = pSimplex->points[2];
        pSimplex->points[2] = pSimplex->points[3];
        pSimplex->size--;
        gjkHandleTriangle(pSimplex, pDirection);
        return false;
    }

    if(sDir(bcd, bo))
    {
        pSimplex->points[0] = pSimplex->points[1];
        pSimplex->points[1] = pSimplex->points[2];
        pSimplex->points[2] = pSimplex->points[3];
        pSimplex->size--;
        gjkHandleTriangle(pSimplex, pDirection);
        return false;
    }

    cout << "FOOKING FOUND IT NIGGERFAGGOT" << endl;
    return true;
}

bool CollisionManager::gjkHandleSimplex(SimplexHelpingStruct* pSimplex, Vec3f* pDirection)
{
    RV_ASSERT(pSimplex->size != 1, "simplex of unexpected size"); // maybe not

    switch (pSimplex->size)
    {
        case 1:
            return gjkHandlePoint(pSimplex, pDirection);
            break;
        case 2:
            return gjkHandleLine(pSimplex, pDirection);
            break;
        case 3:
            return gjkHandleTriangle(pSimplex, pDirection);
            break;
        case 4:
            return gjkHandleTetrahedron(pSimplex, pDirection);
        default:
            RV_ASSERT(false, ""); // should not be in here
            break;
    }

    RV_ASSERT(false, ""); // shouldn't be here
}

void CollisionManager::onUpdateDetectCollisions(float dt)
{
    //detectCollisionsBroadPhase(dt);
    detectCollisionsNarrowPhase(dt);
}

//void CollisionManager::detectCollisionsBroadPhase(float dt)
//{
//
//}

void CollisionManager::detectCollisionsNarrowPhase(float dt)
{
    for(unsigned int i=0; i<Scene::getEntityList()->size(); i++)
    {

        Entity* pFirst = &(*Scene::getEntityList())[i];

        if(!Scene::isEntityValid(pFirst))
            continue;

        Collider* pColliderFirst = PhysicsManager::get()->getCollidableFromEntity(pFirst);
        if(pColliderFirst == nullptr)
            continue;

        for(unsigned int j=i+1; j<Scene::getEntityList()->size(); j++)
        {
            Entity* pSecond = &(*Scene::getEntityList())[j];

            if(!Scene::isEntityValid(pSecond))
                continue;

            Collider* pColliderSecond = PhysicsManager::get()->getCollidableFromEntity(pSecond);
            if(pColliderSecond == nullptr)
                continue;

            if(PhysicsManager::get()->getCollidableFromEntity(pSecond) == nullptr)
                continue;

            //cout << "Checking collision for: " << pFirst->entityName << " | " << pSecond->entityName << endl;

            CollisionPoints collisionPoints = pColliderFirst->collide(pColliderSecond, pFirst->get<TransformComponent>(), pSecond->get<TransformComponent>());
            if(collisionPoints.hasCollided)
            {
                cout << "collision detected" << endl;
                //RV_ASSERT(false, ""); //temp
            }
            Collision collision;
            collision.pEntity1 = pFirst;
            collision.pEntity2 = pSecond;
            collision.collisionPoints = collisionPoints; //TODO: unnecessaraly creating collisionPoints and collision, and also unnecessarily copying collisionPoints to collision

            if(collision.collisionPoints.hasCollided)
            {
                collision.pEntity1->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
                collision.pEntity2->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(1, 0, 0));
            }
            else {
                collision.pEntity1->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 1, 0));
                collision.pEntity2->get<ModelComponent>()->model.pMaterials[0]->set("u_Diffuse", Vec3f(0, 1, 0));
            }

            //PhysicsManager::coll
        }
    }
}

CollisionPoints CollisionManager::collideSphereSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderSphere* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(pFirstCollider->relativeRadius - 1.f < 0.0001f, ""); //temp
    RV_ASSERT(pSecondCollider->relativeRadius - 1.f < 0.0001f, ""); //temp

    RV_ASSERT((pFirstTransform->scale.a[0] == pFirstTransform->scale.a[1]) && (pFirstTransform->scale.a[1]  == pFirstTransform->scale.a[2]), "is not actually a sphere");
    RV_ASSERT((pSecondTransform->scale.a[0] == pSecondTransform->scale.a[1]) && (pSecondTransform->scale.a[1]  == pSecondTransform->scale.a[2]), "is not actually a sphere");


    CollisionPoints collisionPoints;
    if(module(pFirstTransform->position - pSecondTransform->position) <= pFirstTransform->scale.a[0] + pSecondTransform->scale.a[0])
    {
        collisionPoints.hasCollided = true;
    }
    else
    {
        collisionPoints.hasCollided = false;
    }

    return collisionPoints;
}

Vec3f ColliderSphere::findFurthestPoint(const Vec3f& direction, TransformComponent* pTransform) const
{
    RV_ASSERT((pTransform->scale.a[0] == pTransform->scale.a[1]) && (pTransform->scale.a[1]  == pTransform->scale.a[2]), "is not actually a sphere");

    return direction * relativeRadius * pTransform->scale.a[0];
}

Vec3f ColliderBox::findFurthestPoint(const Vec3f& direction, TransformComponent* pTransform) const
{
    RV_ASSERT(false, ""); // temp, can be implemented
}

Vec3f ColliderMesh::findFurthestPoint(const Vec3f& direction, TransformComponent* pTransform) const
{
    Mat4 worldMatrix = pTransform->getTransform();

    Vec3f result;

    float max = -FLT_MAX;
    for(int i=0; i<pMesh->m_Vertices.size(); i++) // potential optimization: put all vertex data in memory consequently for better cache coherency
    {
        Vec4f tempPosVec4f = worldMatrix * Vec4f(pMesh->m_Vertices[i].position, 1.f);
        Vec3f tempPosVec3f = {tempPosVec4f.a[0], tempPosVec4f.a[1], tempPosVec4f.a[2]};
        float tempDot = dot(tempPosVec3f, direction);
        if(tempDot > max)
        {
            max = tempDot;
            result = tempPosVec3f;
        }
    }

    return result;
}

Vec3f supportFunction(const Vec3f& direction, Collider* pFirstCollider, TransformComponent* pFirstTransform, Collider* pSecondCollider, TransformComponent* pSecondTransform)
{
    Vec3f a = pFirstCollider->findFurthestPoint(direction, pFirstTransform);
    Vec3f b = pSecondCollider->findFurthestPoint(-direction, pSecondTransform);
    return a - b;
}

CollisionPoints CollisionManager::collideSphereBox(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideSphereMesh(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideBoxMesh(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideBoxBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, ColliderBox* pSecondCollider, TransformComponent* pSecondTransform)
{
    RV_ASSERT(false, ""); // temp

}

CollisionPoints CollisionManager::collideMeshMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, ColliderMesh* pSecondCollider, TransformComponent* pSecondTransform)
{
    cout << endl;
    return CollisionManager::gjk(pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);
}

void CollisionManager::onUpdateResolveCollisions(float dt)
{

}

void log(const Collider& collider)
{ }

void log(const ColliderSphere& collider)
{
    cout << "relatvieRadius: " << collider.relativeRadius << endl;
    log((Collider&)collider);
}

void log(const ColliderBox& collider)
{
    log((Collider&)collider);
}

CollisionPoints ColliderSphere::collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return pSecondCollider->collideSphere(this, pFirstTransform, pSecondTransform);
}

CollisionPoints ColliderBox::collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return pSecondCollider->collideBox(this, pFirstTransform, pSecondTransform);
}

CollisionPoints ColliderMesh::collide(Collider* pSecondCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return pSecondCollider->collideMesh(this, pFirstTransform, pSecondTransform);
}

// ------------

CollisionPoints ColliderSphere::collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereSphere(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderSphere::collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereBox(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderSphere::collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereMesh(this, pSecondTransform, pFirstCollider, pFirstTransform);
}




CollisionPoints ColliderBox::collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereBox(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderBox::collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideBoxBox(this, pSecondTransform, pFirstCollider, pFirstTransform);
}

CollisionPoints ColliderBox::collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideBoxMesh(this, pSecondTransform, pFirstCollider, pFirstTransform);
}


CollisionPoints ColliderMesh::collideSphere(ColliderSphere* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideSphereMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderMesh::collideBox(ColliderBox* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideBoxMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}

CollisionPoints ColliderMesh::collideMesh(ColliderMesh* pFirstCollider, TransformComponent* pFirstTransform, TransformComponent* pSecondTransform)
{
    return CollisionManager::collideMeshMesh(pFirstCollider, pFirstTransform, this, pSecondTransform);
}