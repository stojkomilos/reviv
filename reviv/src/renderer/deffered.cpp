#include"deffered.h"
#include"core/application.h"

void Deffered::init(unsigned int gBufferWidth, unsigned int gBufferHeight)
{
    initGBuffer(gBufferWidth, gBufferHeight);
}

void Deffered::initGBuffer(unsigned int gBufferWidth, unsigned int gBufferHeight)
{
    m_Width = gBufferWidth;
    m_Height = gBufferHeight;

    gBuffer.init();
    gBuffer.bind();

    //gDepth.init();
    //gDepth.bind(0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // using stencil even though i don't use it because of driver comptability sutff
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth.id, 0);

    gPosition.init();
    gPosition.bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition.id, 0);

    gNormal.init();
    gNormal.bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal.id, 0);

    gAlbedoSpecular.init();
    gAlbedoSpecular.bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecular.id, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; // promeniti?
    glDrawBuffers(3, attachments); // TODO: mozda povecati?

    // TODO: ?
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    RV_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "incomplete framebuffer");

    gBuffer.unbind();
}

void Deffered::resize(unsigned int gBufferWidth, unsigned int gBufferHeight)
{
    gBuffer.~Framebuffer();

    gPosition.~Texture2D();
    gNormal.~Texture2D();
    gAlbedoSpecular.~Texture2D();
    //gDepth.~Texture2D();

    initGBuffer(gBufferWidth, gBufferHeight);
}