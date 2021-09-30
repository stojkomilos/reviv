#pragma once

// gjk algoritm for colliding 2 convex things.

#include"collision.h"

// can be further optimised. namely, this guys has less checks for triangle case: https://www.youtube.com/watch?v=MDusDn8oTSE 
//                                                  also maybe has some optimization: https://www.youtube.com/watch?v=Qupqu1xe7Io 

// some triangle cases may be wrong!

namespace gjkEpa
{

    struct SupportFunctionVertex
    {
        Vec3 vertexDifference;
        Vec3 vertexFirst; // used for EPA
    };

    struct SimplexHelpingStruct
    {
        SupportFunctionVertex points[4];
        unsigned char size;
    };

    // used for both, gjk anmd epa
    SupportFunctionVertex doSupportFunction(const Vec3& direction, const Collider* pFirstCollider, const Collider* pSecondCollider);

    // the function to call from outside
    CollisionPoints doGjkBool(const Collider* pFirstCollider, const Collider* pSecondCollider);
    // ---

    // checks if 2 vectors are pointing in the same direction / same half-plane
    inline bool sDir(const Vec3& first, const Vec3& second) { return dot(first, second) > 0.f; } 

    bool gjkHandleSimplex(SimplexHelpingStruct* pSimplex, Vec3* pDirection);

    void gjkHandleLine(SimplexHelpingStruct* pSimplex, Vec3* pDirection);
    void gjkHandleTriangle(SimplexHelpingStruct* pSimplex, Vec3* pDirection);
    bool gjkHandleTetrahedron(SimplexHelpingStruct *pSimplex, Vec3* pDirection);


    struct Face
    {
        unsigned int a, b, c;
    };

    struct Edge
    {
        unsigned int a, b;
    };

    CollisionPoints doEpa(const SimplexHelpingStruct* pSimplex, const Collider* pFirstCollider, const Collider* pSecondCollider);
    void epaCalculateFaceNormals(std::vector<Vec3>* pNormals, const std::vector<SupportFunctionVertex>& vertices, std::vector<Face>* pFaces);
    void epaGetNearestFace(float* pTempMinFaceDistance, unsigned int* pTempIndexMinFace, const std::vector<Face>& faces, const std::vector<Vec3>& normals, const std::vector<SupportFunctionVertex>& vertices);
    void epaAddEdgeIfUnique(std::vector<Edge>* uniqueEdges, const Edge& edge);
}