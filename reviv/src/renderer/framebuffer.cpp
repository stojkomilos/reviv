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

    delete pTexture;
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

void Framebuffer::attachRenderbuffer()
{
    RV_ASSERT(isInited, "");

    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glGenRenderbuffers(1, &rbo);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    int windowWidth = Application::get()->getWindow()->m_Data.width;
    int windowHeight = Application::get()->getWindow()->m_Data.height;
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight); ///

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    RV_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "incomplete framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::attachTexture()
{
    RV_ASSERT(isInited, "");
    RV_ASSERT(pTexture == nullptr, "texture already added to the framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, id);

    pTexture = new Texture2D();
    
    pTexture->init();
    pTexture->bind(0);

    int windowWidth = Application::get()->getWindow()->m_Data.width;
    int windowHeight = Application::get()->getWindow()->m_Data.height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); ///
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pTexture->id, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, pTexture->id, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pTexture->id, 0);

    RV_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "incomplete framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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