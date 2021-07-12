#include "render_command.h"

#include "core/core.h"

void RenderCommand::drawArrays(const Vao& object) {
	glDrawArrays(GL_TRIANGLES, 0, object.nrOfTriangles * 3);
}
void RenderCommand::setClearColor(Vec4f color)
{
	glClearColor(color.x, color.y, color.z, color.w);
}
void RenderCommand::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::init()
{

#if RV_DEBUG
    glDebugMessageCallback(openGlLogMessageCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // enables you too look in the call stack
#endif
   
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}