#include "vertex_array.h"

using std::cin; using std::cout; using std::endl;

void log(const Vao& vao)
{
    cout << "Vao log() NOT DEFINED" << endl;
}

void Vao::init(const std::string& modelPath)
{
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

    std::vector<BufferElement> tempVboLayout1 = {
        {ShaderDataType::SdtFloat3, "a_Position", false},
        {ShaderDataType::SdtFloat2, "a_TexCoord", false},
        {ShaderDataType::SdtFloat3, "a_Normal",   false},
    };
    BufferLayout vboLayout1(tempVboLayout1);

    assert(sizeof(float) * 3 == sizeof(Vec3f));
    assert(sizeof(int) * 3 == sizeof(TripletOfInts));

    int nrAttributes;											 //samo jedna provere dal ima otprilike dovoljno atrib pointera
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    assert(nrAttributes >= 10); 

    bind(); //TODO: nepotrebna komanda? svakako bind-ujem malo ranije

    pModel = Model::LoadModel()
    vbo.layout = vboLayout1;
    vbo.init((void*)pModel, pModel->nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f)), 0);

    vbo.bind(); //TODO: ne treba?

    addVertexBuffer(vbo);

}
    //stanicTexture.setUp("../resources/textures/stene.png");
    //stanicTexture.bind(0);
    //beloTexture.setUp("../resources/textures/belo.png");


Vao::~Vao()
{
	std::cout << "TODO: dodaj destruktor za vao---\n";
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

	nrOfTriangles = vertexBuffer.count; // works ONLY if you have one Vbo per one Vao
	
}