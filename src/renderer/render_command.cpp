#include "render_command.h"
#include "opengl_assert.h"

void RenderCommand::drawArrays(const VertexArray& object) {
	GLCall(glDrawArrays(GL_TRIANGLES, 0, object.nrOfTriangles * 3));
}
void RenderCommand::setClearColor(mat::vec4 color)
{
	GLCall(glClearColor(color.x, color.y, color.z, color.w));
}
void RenderCommand::clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}