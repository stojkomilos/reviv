#include "vertex_array.h"

using std::cin; using std::cout; using std::endl;
void log(const Vao& vao)
{
    cout << "Vao log() NOT DEFINED" << endl;
}

Vao::Vao() : nrOfTriangles(0)
{
}
Vao::~Vao()
{
	std::cout << "dodaj destruktor za vao---\n";
}
void Vao::setUp()
{
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}
void Vao::bind() const
{
	glBindVertexArray(ID);
}
void Vao::addVertexBuffer(Vbo& vertexBuffer)
{
	for (int i = 0; i < vertexBuffer.layout.elements.size(); i++)
	{
		glVertexAttribPointer(i,
			vertexBuffer.layout.elements[i].getElementCount(),
			shaderDataTypeToOpenGLBaseType(vertexBuffer.layout.elements[i].type),
			vertexBuffer.layout.elements[i].normalized,
			vertexBuffer.layout.stride,
			(void*)vertexBuffer.layout.elements[i].offset);

		glEnableVertexAttribArray(i);
	}

	assert(nrOfTriangles == 0); //treba dodati nacin da vao skonta kolko trouglova ima ako budes dodavao vise vbo za jedan vao, tipa instanced rendereding
	nrOfTriangles = vertexBuffer.count; ///nrOfTrinagles je fucked up ako imas vise od jednog vertex buffera vezanog za jedan vertex array
	
}
