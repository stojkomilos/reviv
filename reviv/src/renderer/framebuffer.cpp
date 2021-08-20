#include"framebuffer.h"
#include"core/application.h"
#include"GLFW/glfw3.h"

void Framebuffer::init()
{
    RV_ASSERT(isInited == false, "");
    isInited = true;
    glGenFramebuffers(1, &id);
}

Framebuffer::~Framebuffer()
{
    RV_ASSERT(isInited, "");
    glDeleteFramebuffers(1, &id);
    isInited = false;
}

void Framebuffer::bind()
{
    RV_ASSERT(isInited, "");
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::unbind()
{
    RV_ASSERT(isInited, "");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
