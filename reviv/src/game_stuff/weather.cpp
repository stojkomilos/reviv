#include"weather.h"

#include"renderer/render_manager.h"

void Weather::init(const std::string& baseNameEntities, float timeInHours)
{
    RV_ASSERT(isInited == false, "already initiazlied");
    isInited = true;

    m_baseNameEntities = baseNameEntities;
    pSunModel = Scene::createEntity(baseNameEntities + "Model");
    pSunDirectionalLight = Scene::createEntity(baseNameEntities + "DirectionalLight");
    
    pSunModel->add<ModelComponent>(&AssetManager::get()->modelLoaderSphere, &RenderManager::get()->shaderMonochroma);
    pSunModel->get<ModelComponent>()->model.pMaterials[0]->set("u_Color", Vec3(1, 1, 0));

    pSunDirectionalLight->add<DirectionalLightComponent>();
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.getShadowMap()->width = 50;
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.getShadowMap()->nearRenderDistance = 0.01f;
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.getShadowMap()->renderDistance = 50.f;
    pSunDirectionalLight->get<DirectionalLightComponent>()->light.enableShadowMap();
    
    setSunTimeOfDay(timeInHours);
}

void Weather::setSunTimeOfDay(float timeInHours)
{
    RV_ASSERT(isInited == true, "can't set when class is not initiazlied");
    m_TotalTimeInHours = timeInHours;
    setSunModelPosition();
    setSunDirectionalLight();
}

void Weather::onUpdate()
{
    if(isInited == true)
    {
        setSunModelPosition();
        setSunDirectionalLight();
    }
}

void Weather::setSunModelPosition()
{
    float angle = m_TotalTimeInHours / 24.f * 2.f * 3.14f;

    sunDirection = {sin(angle), 0, -cos(angle)};

    Vec3 cameraPosition = Scene::getCameraEntity()->get<TransformComponent>()->getPosition();

    float distanceFromCamera = Scene::getCameraEntity()->get<CameraComponent>()->camera.renderDistance - 10;
    *pSunModel->get<TransformComponent>()->getPositionPtr() = cameraPosition + sunDirection * distanceFromCamera;
    pSunModel->get<TransformComponent>()->setScale(Vec3(1, 1, 1) * distanceFromCamera / 10);
}

void Weather::setSunDirectionalLight()
{
    float angle = m_TotalTimeInHours / 24.f * 2.f * 3.14f;

    sunDirection = {sin(angle), 0, -cos(angle)};

    Vec3 cameraPosition = Scene::getCameraEntity()->get<TransformComponent>()->getPosition();

    float distanceFromCamera = 0.5f * pSunDirectionalLight->get<DirectionalLightComponent>()->light.getShadowMap()->renderDistance;
    *pSunDirectionalLight->get<TransformComponent>()->getPositionPtr() = cameraPosition + sunDirection * distanceFromCamera;
    *pSunDirectionalLight->get<TransformComponent>()->getRotationPtr()->getPtr(1, 0) = angle + 3.14f / 2.f;
    *pSunDirectionalLight->get<TransformComponent>()->getRotationPtr()->getPtr(2, 0) = 0;
    *pSunDirectionalLight->get<TransformComponent>()->getRotationPtr()->getPtr(1, 0) = 0;
}