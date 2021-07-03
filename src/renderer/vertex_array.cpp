#include "vertex_array.h"

VertexArrayComponent::VertexArrayComponent() : nrOfTriangles(0)
{
}
VertexArrayComponent::~VertexArrayComponent()
{
	std::cout << "dodaj destruktor za vao---\n";
}
void VertexArrayComponent::setUp() {
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}
void const VertexArrayComponent::bind()
{
	glBindVertexArray(ID);
}
void VertexArrayComponent::addVertexBuffer(VertexBufferComponent& vertexBuffer)
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
