#include"mesh.h"


void log(const Mesh& mesh)
{
    cout << "vertices.size: " << mesh.m_Vertices.size() << " indices: " << mesh.m_Indices.size() << endl;
}

void log(const Vertex& vertex)
{
    cout << "Vertex:---" << endl;
    cout << "Position: ";
    log(vertex.position);
    cout << "normal: ";
    log(vertex.normal);
    cout << "texCoords: ";
    log(vertex.texCoords);
    cout << "---" << endl;
}