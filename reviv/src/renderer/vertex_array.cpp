#include "vertex_array.h"

using std::cin; using std::cout; using std::endl;

void log(const Vao& vao)
{
    cout << "Vao log() NOT DEFINED" << endl;
}

Vao::Vao(const std::string& modelPath)
{
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

    //bind(); //NOVO: izgleda da treba

    std::vector<BufferElement> tempVboLayout1 = {
        {ShaderDataType::SdtFloat3, "a_Position", false},
        {ShaderDataType::SdtFloat2, "a_TexCoord", false},
        {ShaderDataType::SdtFloat3, "a_Normal",   false},
    };
    BufferLayout vboLayout1(tempVboLayout1);

    RV_ASSERT(sizeof(float) * 3 == sizeof(Vec3f), "");
    RV_ASSERT(sizeof(int) * 3 == sizeof(TripletOfInts), "");

    int nrAttributes;											 //samo jedna provere dal ima otprilike dovoljno atrib pointera
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    RV_ASSERT((nrAttributes >= 10), ""); 

    //bind(); //TODO: nepotrebna komanda? svakako bind-ujem malo ranije

    pModel = new Model;
    pModel->init(modelPath.c_str());
    
    vbo.layout = vboLayout1;
    vbo.init((void*)pModel->pointer, pModel->nrTriangles * 3 * (2 * sizeof(Vec3f) + sizeof(Vec2f)), 0);

    //vbo.bind(); //TODO: ne treba?

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