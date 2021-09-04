#include"gjk_epa.h"

#include"scene/components.h"
#include<cfloat>

// TODO: optimization: smanji mnoogo korisititi -Vec3f, moze se obrnuti na cross productu i dot productu(sDir) free

namespace gjkEpa
{

    Vec3f doSupportFunction(const Vec3f& direction, Collider* pFirstCollider, TransformComponent* pFirstTransform, Collider* pSecondCollider, TransformComponent* pSecondTransform)
    {
        return pFirstCollider->findFurthestPoint(direction, pFirstTransform) - pSecondCollider->findFurthestPoint(-direction, pSecondTransform);
    }

    CollisionPoints doGjkDetectCollision(Collider* pFirstCollider, TransformComponent* pFirstTransform, Collider* pSecondCollider, TransformComponent* pSecondTransform)
    {
        Vec3f direction = pSecondTransform->position - pFirstTransform->position;

        Vec3f point1 = doSupportFunction(direction, pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);

        SimplexHelpingStruct simplex;
        simplex.points[0] = point1;
        simplex.size = 1;

        CollisionPoints result;
        result.hasCollided = false;

        if(!sDir(direction, simplex.points[0]))
        {
            return result;
        }

        direction = -simplex.points[0];

        while(true)
        {
            Vec3f newPoint = doSupportFunction(direction, pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);

            if(!sDir(direction, newPoint))
            {
                break;
            }
            if(module(newPoint) < 0.0001f)
            {
                RV_ASSERT(false, "");
            }

            RV_ASSERT(simplex.size >= 0 && simplex.size <= 3, "unexpected simplex size");
            simplex.points[simplex.size] = newPoint;
            simplex.size++;

#ifdef RV_DEBUG
            for(int i=0; i<simplex.size; i++)
                for(int j=i+1; j<simplex.size; j++)
                {
                    RV_ASSERT(module(simplex.points[i] - simplex.points[j]) > 0.001f, "");
                }
#endif
            if(gjkHandleSimplex(&simplex, &direction))
            {
                result = doEpa(&simplex, pFirstTransform, pFirstCollider, pSecondTransform, pSecondCollider);
                result.hasCollided = true;
                break;
            }
        }

        return result;
    }

    void gjkHandleLine(SimplexHelpingStruct* pSimplex, Vec3f* pDirection)
    {
        RV_ASSERT(pSimplex->size == 2, "invalid simplex size");

        Vec3f a = pSimplex->points[0];
        Vec3f b = pSimplex->points[1];
        Vec3f ab = b - a;
        Vec3f ao = -a;
        Vec3f bo = -b;

        RV_ASSERT(!sDir(-ab, ao), "invalid origin location");

        if(sDir(-ab, bo))
        {
            *pDirection = cross(cross(ab, ao), ab);
        }
        else {
            pSimplex->points[0] = pSimplex->points[1];
            pSimplex->size = 1;
            *pDirection = -pSimplex->points[0];

            RV_ASSERT(sDir(ab, bo), "invalid origin location"); // redundant?
        }
    }

    void gjkHandleTriangle(SimplexHelpingStruct* pSimplex, Vec3f* pDirection)
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
        Vec3f bcPerp = cross(bc, abc);

        RV_ASSERT(!(sDir(ao, ab) && sDir(bo, -ab) && sDir(cross(abc, -ab), ao)), "invalid origin location"); // region behind ab(my naming convention)
        //RV_ASSERT(!sDir(ab, bo) && !sDir(-ab, ao), ""); // can sometimes get triggered because of floating point innacuracies
        RV_ASSERT(!(sDir(-ac, ao) && sDir(co, cross(abc, ac))) && !(sDir(-bc, bo) && sDir(co, cross(bc, abc))) && !sDir(cross(ab, abc), ao), "origin not in the 3 voronoi regions that it should be in");

        if(sDir(acPerp, ao))
        {
            if(sDir(-ac, co))
            {
                pSimplex->size = 2;
                pSimplex->points[1] = pSimplex->points[2];
                *pDirection = cross(cross(ac, ao), ac);

                RV_ASSERT(sDir(*pDirection, ao) && sDir(ao, ac) && sDir(co, -ac), "");
                RV_ASSERT(sDir(acPerp, ao) && sDir(acPerp, co) && sDir(acPerp, *pDirection), "");
            }
            else {
                pSimplex->size = 2;
                pSimplex->points[0] = pSimplex->points[1];
                pSimplex->points[1] = pSimplex->points[2];
                gjkHandleLine(pSimplex, pDirection);
            }
        }
        else if(sDir(bcPerp, bo))
        {
            pSimplex->size = 2;
            pSimplex->points[0] = pSimplex->points[1];
            pSimplex->points[1] = pSimplex->points[2];
            gjkHandleLine(pSimplex, pDirection);
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
            }
        }

    }

    bool gjkHandleTetrahedron(SimplexHelpingStruct *pSimplex, Vec3f* pDirection)
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

        Vec3f ao = -a;
        Vec3f bo = -b;
        Vec3f co = -c;
        Vec3f doo = -d;


        Vec3f abd = cross(ab, ad);
        Vec3f adc = cross(ad, ac);
        Vec3f bcd = cross(bc, bd);

        // probably can be removed

        Vec3f abc = cross(ab, ac);
        Vec3f acPerp = cross(abc, ac);
        Vec3f bcPerp = cross(bc, abc);

        RV_ASSERT(sDir(*pDirection, ao), "invalid origin position");
        RV_ASSERT(sDir(abc, ao), "invalid origin position");

#ifdef RV_DEBUG // makes a point inside the simplex and checks the face normal directoins relative to the created point
    Vec3f bcHalf = b + bc * 0.5;
    Vec3f point = a + (bcHalf - a) * 0.5;
    point = point + (d - point) * 0.5f;
    point = point - d;
    RV_ASSERT(sign(dot(abd, point)) == sign(dot(adc, point)) && sign(dot(adc, point)) == sign(dot(bcd, point)), "");
    point = point + d - a;
    RV_ASSERT(sign(dot(abd, point)) != sign(dot(abc, point)), "");
#endif

        bool isAbd = sDir(abd, ao);
        bool isAdc = sDir(adc, ao);
        bool isBcd = sDir(bcd, bo);

        RV_ASSERT(!(isAbd && isAdc && isBcd), ""); // detected that origin is in voronoi regions of all 3 sides

        RV_ASSERT(!(isAbd && isAdc && isBcd), ""); // impossible geometrically that all 3 of theese are true. check on paper

        if(isAbd && !isAdc && !isBcd)
        {
            pSimplex->size = 3;

            pSimplex->points[2] = pSimplex->points[3];

            gjkHandleTriangle(pSimplex, pDirection);
            return false;
        }

        if(!isAbd && isAdc && !isBcd)
        {
            pSimplex->size = 3;

            pSimplex->points[0] = c;
            pSimplex->points[1] = a;
            pSimplex->points[2] = d;

            gjkHandleTriangle(pSimplex, pDirection);
            return false;
        }

        if(!isAbd && !isAdc && isBcd)
        {
            pSimplex->size = 3;

            pSimplex->points[0] = b;
            pSimplex->points[1] = c;
            pSimplex->points[2] = d;

            gjkHandleTriangle(pSimplex, pDirection);
            return false;
        }

        /// 2-cases

        if(isAbd && isAdc && !isBcd)
        {
            if(sDir(cross(ad, abd), doo))
            {
                pSimplex->size = 3;
                pSimplex->points[2] = pSimplex->points[3];
            }
            else {
                pSimplex->size = 3;

                pSimplex->points[0] = c;
                pSimplex->points[1] = a;
                pSimplex->points[2] = d;
            }
            gjkHandleTriangle(pSimplex, pDirection);
            return false;
        }

        if(isAbd && !isAdc && isBcd)
        {
            if(sDir(cross(bd, abd), doo))
            {
                pSimplex->size = 3;
                pSimplex->points[2] = pSimplex->points[3];
            }
            else {
                pSimplex->size = 3;

                pSimplex->points[0] = b;
                pSimplex->points[1] = c;
                pSimplex->points[2] = d;
            }
            gjkHandleTriangle(pSimplex, pDirection);
            return false;
        }


        if(!isAbd && isAdc && isBcd)
        {
            if(sDir(cross(cd, adc), doo))
            {
                pSimplex->size = 3;

                pSimplex->points[0] = c;
                pSimplex->points[1] = a;
                pSimplex->points[2] = d;

            }
            else {
                pSimplex->size = 3;

                pSimplex->points[0] = b;
                pSimplex->points[1] = c;
                pSimplex->points[2] = d;
                
            }

            gjkHandleTriangle(pSimplex, pDirection);
            return false;
        }


        RV_ASSERT(!isAdc && !isAbd && !isBcd, "");
        RV_ASSERT(sDir(cross(ab, ac), ao) && sDir(-abd, ao) && sDir(-adc, ao) && sDir(-bcd, bo), "point not inside simplex, but it should be");

        return true;
    }

    bool gjkHandleSimplex(SimplexHelpingStruct* pSimplex, Vec3f* pDirection)
    {
        RV_ASSERT(pSimplex->size != 1, "simplex of unexpected size"); // maybe not

        switch (pSimplex->size)
        {
            case 2:
                gjkHandleLine(pSimplex, pDirection);
                break;
            case 3:
                gjkHandleTriangle(pSimplex, pDirection);
                break;
            case 4:
                return gjkHandleTetrahedron(pSimplex, pDirection);
            default:
                RV_ASSERT(false, ""); // should not be in here
                break;
        }

        RV_ASSERT(pSimplex->size != 4, "");
        return false;
    }

    CollisionPoints doEpa(SimplexHelpingStruct* pSimplex, TransformComponent* pFirstTransform, Collider* pFirstCollider, TransformComponent* pSecondTransform, Collider* pSecondCollider)
    {
        std::vector<Face> faces;        // all theese can be put as dynamics arrays into Collider structure for performance
        std::vector<Vec3f> vertices;

        faces.push_back({0, 1, 2});
        faces.push_back({0, 1, 3});
        faces.push_back({0, 3, 2});
        faces.push_back({1, 2, 3});

        vertices.push_back(pSimplex->points[0]);
        vertices.push_back(pSimplex->points[1]);
        vertices.push_back(pSimplex->points[2]);
        vertices.push_back(pSimplex->points[3]);

        CollisionPoints result;

        while(true)
        {

#ifdef RV_DEBUG
    for(int i=0; i<vertices.size(); i++)
        for(int j=i+1; j<vertices.size(); j++)
        {
            RV_ASSERT(module(vertices[i] - vertices[j]) > 0.001f, "");
        }
#endif

            std::vector<Vec3f> normals;
            // calculate normals
            epaCalculateFaceNormals(&normals, vertices, &faces);

            // find nearest face
            float tempMinFaceDistance = FLT_MAX;
            unsigned int tempIndexMinFace;

            epaGetNearestFace(&tempMinFaceDistance, &tempIndexMinFace, faces, normals, vertices);

            Vec3f supportPoint = doSupportFunction(normals[tempIndexMinFace], 
                pFirstCollider, pFirstTransform, pSecondCollider, pSecondTransform);

            if(abs(dot(normals[tempIndexMinFace], supportPoint) - tempMinFaceDistance) > 0.0001f)
            { 
                // invalid face

                int indexNewVertex = -1;

                //for(int i=0; i<4; i++)
                for(int i=0; i<4; i++)
                    if(compare(supportPoint, vertices[i], 0.001f))
                    {
                        indexNewVertex = i;
                        cout << "indexNewVertex: " << indexNewVertex << endl;
                        break;
                    }

                if(indexNewVertex == -1)
                {
                    vertices.push_back(supportPoint);
                    indexNewVertex = vertices.size() - 1;
                }

                cout << "adding supportPoint: ";
                log(supportPoint);

                std::vector<Edge> edges;
                std::vector<unsigned int> facesToErase;

                RV_ASSERT(faces.size() == normals.size(), "");

                // invalidate all faces pointing in the same half plane as supportPoint
                for(int i=0; i<faces.size(); i++)
                {
                    if(dot(normals[i], supportPoint - vertices[faces[i].a]) > 0)
                    {
                        epaAddEdgeIfUnique(&edges, {faces[i].a, faces[i].b});
                        epaAddEdgeIfUnique(&edges, {faces[i].b, faces[i].c});
                        epaAddEdgeIfUnique(&edges, {faces[i].c, faces[i].a});

                        facesToErase.push_back(i);
                    }
                }

                // remove all non unique faces
                for(int i=0; i<facesToErase.size(); i++)
                {
                    faces.erase(faces.begin() + facesToErase[i] - i);
                }

                cout << "facesToErase.size(): " << facesToErase.size() << " edges.size(): " << edges.size() << endl;

                // add new faces from unique edges
                for(int i=0; i<edges.size(); i++)
                {
                    faces.push_back({edges[i].a, edges[i].b, indexNewVertex});
                }

            }
            else { // valid face
                result.depth = tempMinFaceDistance;
                result.normal = normals[tempIndexMinFace] / module(normals[tempIndexMinFace]);
                break;
            }
        }

        return result;
    }

    void epaCalculateFaceNormals(std::vector<Vec3f>* pNormals, const std::vector<Vec3f>& vertices, std::vector<Face>* pFaces)
    {
        for(int i=0; i<pFaces->size(); i++)
        {
            Vec3f ab = vertices[(*pFaces)[i].b] - vertices[(*pFaces)[i].a];
            Vec3f ac = vertices[(*pFaces)[i].c] - vertices[(*pFaces)[i].a];
            Vec3f normal = cross(ab, ac);

            if(module(normal) < 0.0001f)
            {
                cout << endl;
                RV_ASSERT(false, ""); // TODO: ovo se calluje nekad
            }

            normal = normal / module(normal);

            if(dot(normal, vertices[(*pFaces)[i].b]) < 0)
            {
                normal = -normal;
                cout << "Reversing normal" << endl;

                int help = (*pFaces)[i].a;
                (*pFaces)[i].a = (*pFaces)[i].b;
                (*pFaces)[i].b = help;
            }

            pNormals->push_back(normal);
        }
    }


    void epaAddEdgeIfUnique(std::vector<Edge>* pUniqueEdges, const Edge& edge)
    {
        int indexToErase = -1;
        for(int i=0; i<pUniqueEdges->size(); i++)
        {
            if((*pUniqueEdges)[i].a == edge.b && (*pUniqueEdges)[i].b == edge.a)
            {
                RV_ASSERT(indexToErase == -1, ""); // TODO: ovo se calluje nekad
                indexToErase = i;
                break;
            }
        }
        if(indexToErase == -1)
        {
                pUniqueEdges->push_back(edge);
        }
        else {
            pUniqueEdges->erase(pUniqueEdges->begin() + indexToErase);
        }
    }

    void epaGetNearestFace(float* pTempMinFaceDistance, unsigned int* pTempIndexMinFace, const std::vector<Face>& faces, const std::vector<Vec3f>& normals, const std::vector<Vec3f>& vertices)
    {
        for(int i=0; i<faces.size(); i++)
        {
            float distanceToFace = dot(normals[i], vertices[faces[i].a]);

            if(distanceToFace < *pTempMinFaceDistance)
            {
                *pTempIndexMinFace = i;
                *pTempMinFaceDistance = distanceToFace;
            }
        }
    }
}
