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
 : ambient(1, 1, 1), diffuse(0.2, 0.2, 0.2), specular(0.5, 0.5, 0.5), on(true), intensity(1.f)
{ }

PointLight::PointLight()
 : constant(1.f), linear(0.14f), quadratic(0.07f) // range approximately 32 meters
{ }