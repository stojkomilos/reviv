#include"light.h"

void log(const Light& light)
{
    cout << "ambient: ";
    log(light.ambient);
    cout << "diffuse: ";
    log(light.diffuse);
    cout << "specular: ";
    log(light.specular);
}

void log(const DirectionalLight& light)
{
    log((const Light&)light);
}

void log(const PointLight& light)
{
    cout << "constant: " << light.constant << endl;
    cout << "linear: " << light.constant << endl;
    cout << "quadratic: " << light.constant << endl;
    log((const Light&)light);
}
 
void ShadowMapDirectional::init(unsigned int resolutionWidth, unsigned int resolutionHeight)
{
    m_ResolutionWidth = resolutionWidth;
    m_ResolutionHeight = resolutionHeight;
    
    framebuffer.init();
    depthMap.init();
    depthMap.bind(0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_ResolutionWidth, m_ResolutionHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.f, 1.f, 1.f, 1.f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    framebuffer.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    framebuffer.unbind();

    camera.renderDistance = renderDistance;
    camera.nearRenderDistance = nearRenderDistance;
}

void ShadowMapOmnidirectional::init(unsigned int resolutionWidth, unsigned int resolutionHeight)
{
    m_ResolutionWidth = resolutionWidth;
    m_ResolutionHeight = resolutionHeight;

    framebuffer.init(); // ?
    depthMap.init();
    depthMap.bind(0); // ?

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 
        m_ResolutionWidth, m_ResolutionHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    depthMap.unbind();

    shadowMap.init();
    shadowMap.bind(0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for(int face=0; face<6; face++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_R32F, 
            m_ResolutionWidth, m_ResolutionHeight, 0, GL_RED, GL_FLOAT, NULL); // TODO: (very optional) don't use the red buffer, but just use the other floating point stuff?
    }

    framebuffer.bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.id, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    RV_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "incomplete framebuffer");

    framebuffer.unbind();

    camera.renderDistance = renderDistance;
    camera.nearRenderDistance = nearRenderDistance;

}

/* ovo je za nacin preko geometry shadera
void ShadowMapOmnidirectional::init(unsigned int resolutionWidth, unsigned int resolutionHeight)
{
    m_ResolutionWidth = resolutionWidth;
    m_ResolutionHeight = resolutionHeight;

    framebuffer.init(); // ?
    depthMap.init();
    depthMap.bind(0); // ?

    for(int i=0; i<6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
            m_ResolutionWidth, m_ResolutionHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    framebuffer.bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap.id, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    framebuffer.unbind();

    camera.renderDistance = renderDistance;
    camera.nearRenderDistance = nearRenderDistance;
}
*/

void DirectionalLight::enableShadowMap()
{
    RV_ASSERT(isShadowMapped == false, "shadow mapping for this light is already on");
    isShadowMapped = true;
    shadowMap.init(1024, 1024);
}

void PointLight::enableShadowMap()
{
    RV_ASSERT(isShadowMapped == false, "shadow mapping for this light is already on");
    isShadowMapped = true;
    shadowMap.init(1024, 1024);
}

ShadowMap* DirectionalLight::getShadowMap()
{
    return &shadowMap;
}

ShadowMap* PointLight::getShadowMap()
{
    return &shadowMap;
}

Texture* ShadowMapDirectional::getDepthMap()
{
    return &depthMap;
}

Texture* ShadowMapOmnidirectional::getDepthMap()
{
    return &depthMap;
}