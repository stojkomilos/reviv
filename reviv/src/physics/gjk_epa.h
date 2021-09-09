#pragma once

// gjk algoritm for colliding 2 convex things.

#include"collision_manager.h"

// can be further optimised. namely, this guys has less checks for triangle case: https://www.youtube.com/watch?v=MDusDn8oTSE 
//                                                  also maybe has some optimization: https://www.youtube.com/watch?v=Qupqu1xe7Io 

// some triangle cases may be wrong!


namespace gjkEpa
{

    struct SupportFunctionVertex
    {
        Vec3f vertexDifference;
        Vec3f vertexFirst; // used for EPA
    };

    struct SimplexHelpingStruct
    {
        SupportFunctionVertex points[4];
        unsigned char size;
    };

    // used for both, gjk anmd epa
    SupportFunctionVertex doSupportFunction(const Vec3f& direction, Collider* pFirstCollider, TransformComponent* pFirstTransform, Collider* pSecondCollider, TransformComponent* pSecondTransform);

    // the function to call from outside
    CollisionPoints doGjkDetectCollision(Collider* pFirstCollider, TransformComponent* pFirstTransform, Collider* pSecondCollider, TransformComponent* pSecondTransform);
    // ---

    // checks if 2 vectors are pointing in the same direction / same half-plane
    inline bool sDir(const Vec3f& first, const Vec3f& second) { return dot(first, second) > 0.f; } 

    bool gjkHandleSimplex(SimplexHelpingStruct* pSimplex, Vec3f* pDirection);

    void gjkHandleLine(SimplexHelpingStruct* pSimplex, Vec3f* pDirection);
    void gjkHandleTriangle(SimplexHelpingStruct* pSimplex, Vec3f* pDirection);
    bool gjkHandleTetrahedron(SimplexHelpingStruct *pSimplex, Vec3f* pDirection);


    struct Face
    {
        unsigned int a, b, c;
    };

    struct Edge
    {
        unsigned int a, b;
    };

    CollisionPoints doEpa(SimplexHelpingStruct* pSimplex, TransformComponent* pFirstTransform, Collider* pFirstCollider, TransformComponent* pSecondTransform, Collider* pSecondCollider);
    void epaCalculateFaceNormals(std::vector<Vec3f>* pNormals, const std::vector<SupportFunctionVertex>& vertices, std::vector<Face>* pFaces);
    void epaGetNearestFace(float* pTempMinFaceDistance, unsigned int* pTempIndexMinFace, const std::vector<Face>& faces, const std::vector<Vec3f>& normals, const std::vector<SupportFunctionVertex>& vertices);
    void epaAddEdgeIfUnique(std::vector<Edge>* uniqueEdges, const Edge& edge);
}