#include"weather.h"

#include"renderer/render_manager.h"

void Weather::init(int timeOfDayHours, int timeOfDayMinutes, float timeOfDaySeconds)
{
    pSunModel = Scene::createEntity("WeatherSunModel");
    pSunDirectionalLight = Scene::createEntity("WeatherSunDirectionalLight");
    
    pSunModel->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->shaderMonochroma);
    pSunModel->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec3f(1, 1, 0));

    pSunDirectionalLight->add<DirectionalLightComponent>();
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.shadowMap.width = 20;
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.shadowMap.nearRenderDistance = 0.1f;
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.shadowMap.renderDistance = 20.f;
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.enableShadowMap();
    
    setSunPosition(timeOfDayHours, timeOfDayMinutes, timeOfDaySeconds);
}

void Weather::setSunPosition(int timeOfDayHours, int timeOfDayMinutes, float timeOfDaySeconds)
{
    m_TimeOfDayHours = timeOfDayHours;
    m_TimeOfDayMinutes = timeOfDayMinutes;
    m_TimeOfDaySeconds = timeOfDaySeconds;

    setSunModelPosition();
    setSunDirectionalLightPosition();
}

void Weather::onUpdate()
{
    setSunModelPosition();
}

void Weather::setSunModelPosition()
{
    float totalTime = m_TimeOfDayHours * 60.f * 60.f + m_TimeOfDayMinutes * 60.f + m_TimeOfDaySeconds;

    float partOfDay = totalTime / (24.f * 60.f * 60.f);
    float angle = partOfDay * 2.f * 3.14f;

    sunDirection.a[0] = sin(angle);
    sunDirection.a[1] = 0;
    sunDirection.a[2] = -cos(angle);

    Vec3f cameraPosition = Scene::getCameraEntity()->get<TransformComponent>()->position;

    pSunModel->get<TransformComponent>()->position = cameraPosition + sunDirection * (Scene::getCameraEntity()->get<CameraComponent>()->camera.renderDistance - 10);
}

void Weather::setSunDirectionalLightPosition()
{
    float totalTime = m_TimeOfDayHours * 60.f * 60.f + m_TimeOfDayMinutes * 60.f + m_TimeOfDaySeconds;

    float partOfDay = totalTime / (24.f * 60.f * 60.f);
    float angle = partOfDay * 2.f * 3.14f;

    Vec3f cameraPosition = Scene::getCameraEntity()->get<TransformComponent>()->position;

    pSunModel->get<TransformComponent>()->position = cameraPosition + sunDirection * (Scene::getCameraEntity()->get<CameraComponent>()->camera.renderDistance - 10);
    float modelScaleFactor = (Scene::getCameraEntity()->get<CameraComponent>()->camera.renderDistance - 10) / 10;
    pSunModel->get<TransformComponent>()->scale *= modelScaleFactor;

    float directionalLightScaleFactor = 2.f * pSunDirectionalLight->get<DirectionalLightComponent>()->light.shadowMap.renderDistance;
    pSunDirectionalLight->get<TransformComponent>()->position = cameraPosition + sunDirection * directionalLightScaleFactor;
    pSunDirectionalLight->get<TransformComponent>()->rotation.pitch = angle + 3.14f / 2.f;
    pSunDirectionalLight->get<TransformComponent>()->rotation.yaw = 0;
    pSunDirectionalLight->get<TransformComponent>()->rotation.roll = 0;
}