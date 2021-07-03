#include "render_command.h"

void RenderCommand::drawArrays(const VertexArray& object) {
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
