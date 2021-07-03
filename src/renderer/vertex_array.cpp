#include "vertex_array.h"

VaoComponent::VaoComponent() : nrOfTriangles(0)
{
}
VaoComponent::~VaoComponent()
{
	std::cout << "dodaj destruktor za vao---\n";
}
void VaoComponent::setUp() {
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}
void const VaoComponent::bind()
{
	glBindVertexArray(ID);
}
void VaoComponent::addVertexBuffer(Vbo& vertexBuffer)
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
