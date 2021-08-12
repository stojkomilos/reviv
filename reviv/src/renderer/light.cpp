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
    cout << "direction: ";
    log(light.direction);

    log((Light)light);
}

void log(const PointLight& light)
{
    cout << "constant: " << light.constant << endl;
    cout << "linear: " << light.constant << endl;
    cout << "quadratic: " << light.constant << endl;
    log((Light)light);
}
 
Light::Light()
 : ambient(0.1, 0.1, 0.1), diffuse(0.2, 0.2, 0.2), specular(0.5, 0.5, 0.5), on(true), intensity(1.f)
{ }

PointLight::PointLight()
 : constant(1.f), linear(0), quadratic(0.07f) // range approximately 32 meters
{ }

void ShadowMap::init(unsigned int width, unsigned int height)
{
    m_Width = width;
    m_Height = height;

    glViewport(0, 0, width, height);
    
    framebuffer.init();
    depthMap.init();
    depthMap.bind(0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
}