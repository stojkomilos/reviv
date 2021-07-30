#include "framebuffer.h"

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
}

void Framebuffer::bind()
{
    RV_ASSERT(isInited, "");
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

//void Framebuffer::setUp(FramebufferSpecification& specification1)
//{
//    specification = specification1;
//}
//
//void Framebuffer::invalidate()
//{
//    glCreateFramebuffers(GL_TEXTURE_2D, 1, &id);
//    glBindFramebuffer(GL_FRAMEBUFFER, id);
//
//    glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
//    glBindTexture(GL_TEXTURE_2D, colorAttachment);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width, specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);
//
//    glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
//    glBindTextures(GL_TEXTURE_2D, depthAttachment);
//    glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specification.width, specification.height);
//    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, specification.width, specification.height, 0, GL_DEPTH_STENCIL, GL_UN
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
//
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//    {
//        std::cout << "ERROR Frame buffer is incomplete.\n";
//        assert(false);
//    }
//
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void Framebuffer::bind()
//{
//    glBindFramebuffer(GL_FRAMEBUFFER, id);
//}
//
//void Framebuffer::unbind()
//{
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//