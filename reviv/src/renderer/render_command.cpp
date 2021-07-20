#include "render_command.h"

extern void* gData1;
extern void* gData2;

void logMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParams);

void RenderCommand::iDrawArrays(const Mesh& mesh)
{
    glDrawArrays(GL_TRIANGLES, 0, mesh.m_Vertices.size());
    //RV_ASSERT(false, "");
}

void RenderCommand::iDrawElements(const Mesh& mesh)
{
    //cout << "Size: " << mesh.m_Indices.size() << endl;
    for(int i=0; i < mesh.m_Indices.size(); i++)
    {
        //cout << "i: " << i << "ind: " << mesh.m_Indices[i] << endl;
    }

    auto* jedan = (std::vector<Vertex>*) gData1;
    auto* dva = (std::vector<unsigned int>*) gData2;

    for(int i=0; i < jedan->size(); i++)
        log((*jedan)[i]);
    for(int i=0; i < dva->size(); i++)
        log((*dva)[i]);
    cout << "adsf" << endl;

    glDrawElements(GL_TRIANGLES, mesh.m_Indices.size(), GL_UNSIGNED_INT, (void*)0);

}

void RenderCommand::iSetClearColor(const Vec4f& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void RenderCommand::iClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::iInit()
{

#if RV_DEBUG
    glDebugMessageCallback(logMessageCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // enables you too look in the call stack
    //cout << "Running in DEBUG mode" << endl;
#endif
   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    RV_ASSERT((nrAttributes >= 10), ""); 

}

void logMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParams)
{
    cout << endl;

    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            RV_ASSERT(false, "[OpenGL Debug HIGH]" << message);
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            RV_ASSERT(false, "[OpenGL Debug MEDIUM]" << message);
            break;

        case GL_DEBUG_SEVERITY_LOW:
            RV_ASSERT(false, "[OpenGL Debug LOW]" << message);
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            cout << "[OpenGL Debug NOTIFICATION] " << message << endl;
            break;

        default:
            RV_ASSERT(false, "ERROR: opengl returned unknown debug error SEVERITY");
            break;
    }
}