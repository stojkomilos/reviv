#include"environment.h"
#include"render_manager.h"

void log(const Environment& environment)
{
    cout << "Environment----" << endl;
    log(environment.shaderUniformMap);
    cout << "END of environment" << endl;
}

void Environment::setLights()
{
    nrPointLights = 0;
    nrDirectionalLights = 0;

    Light* pLight;
    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(!itEntity->valid)
            continue;

        if(itEntity->has<PointLightComponent>())                pLight = &itEntity->get<PointLightComponent>()->light;
        else if(itEntity->has<DirectionalLightComponent>())     pLight = &itEntity->get<DirectionalLightComponent>()->light;
        else continue;

        if(pLight->on == false)
        {
            RV_ASSERT(false, ""); // temp, can be removed
            continue;
        }

        std::string uniformNameLight;

        if(pLight->lightType == LightType::LightTypePoint)
        {
            uniformNameLight = "ue_PointLights[";
            uniformNameLight += (char)(nrPointLights + '0');
        }
        else {
            RV_ASSERT(pLight->lightType == LightType::LightTypeDirectional, "light type not recognized");
            uniformNameLight = "ue_DirectionalLights[";
            uniformNameLight += (char)(nrDirectionalLights + '0');
        }
        uniformNameLight += "]";

        set(uniformNameLight + ".ambient", pLight->ambient);
        set(uniformNameLight + ".diffuse", pLight->diffuse);
        set(uniformNameLight + ".specular", pLight->specular);

        if(pLight->lightType == LightType::LightTypePoint)
        {
            set(uniformNameLight + ".position", itEntity->get<TransformComponent>()->position);

            set(uniformNameLight + ".constant", ((PointLight*)pLight)->constant);
            set(uniformNameLight + ".linear", ((PointLight*)pLight)->linear);
            set(uniformNameLight + ".quadratic", ((PointLight*)pLight)->quadratic);
        }
        else {
            Vec3f direction = getDirectionFromRotation(itEntity->get<TransformComponent>()->rotation);
            set(uniformNameLight + ".direction", direction);
        }

        setShadowMap(&(*itEntity), pLight, uniformNameLight);

        if(itEntity->has<PointLightComponent>())
        {
            nrPointLights++;
            RV_ASSERT(nrPointLights < 10, ""); // can be implemented, just string and one digit stuff
        }
        else if(itEntity->has<DirectionalLightComponent>())
        {
            nrDirectionalLights++;
            RV_ASSERT(nrDirectionalLights < 10, ""); // can be implemented, just string and one digit stuff
        }
        else { RV_ASSERT(false, "light type not recognized"); }
    }

    set("ue_NumberOfPointLights", (int)nrPointLights);
    set("ue_NumberOfDirectionalLights", (int)nrDirectionalLights);
}

void Environment::setShadowMap(Entity* pEntity, Light* pLight, const std::string& uniformNameLight)
{
    if(pLight->isShadowMapped)
    {
        set(uniformNameLight + ".isShadowMapped", (int)1);
        if(pLight->lightType == LightType::LightTypeDirectional)
        {
            setTexture(uniformNameLight + ".shadowMap.depthMap", *pLight->getShadowMap()->getDepthMap());

            Camera* pCamera = &pLight->getShadowMap()->camera;
            pCamera->nearRenderDistance = pLight->getShadowMap()->nearRenderDistance;
            pCamera->renderDistance = pLight->getShadowMap()->renderDistance;
            pCamera->setViewMatrix(pEntity->get<TransformComponent>()->position, pEntity->get<TransformComponent>()->rotation);
            if(pLight->lightType == LightType::LightTypePoint)
            {
                pCamera->setPerspectiveProjection(degreesToRadians(90), 1.f);
            }
            else {
                pCamera->setOrthographicProjection(pLight->getShadowMap()->width, 1);
                RV_ASSERT(pLight->getShadowMap()->width > 0, "shadow map is incorrectly or incompletely configured");
            }

            set(uniformNameLight + ".shadowMap.viewMatrix", pLight->getShadowMap()->camera.viewMatrix);
            set(uniformNameLight + ".shadowMap.projectionMatrix", pLight->getShadowMap()->camera.projectionMatrix);

            RV_ASSERT(pLight->getShadowMap()->nearRenderDistance != 0 && pLight->getShadowMap()->renderDistance != 0 && pLight->getShadowMap()->nearRenderDistance < pLight->getShadowMap()->renderDistance, "shadow map is incorrectly or incompletely configured");
        }
        else {
            const std::string baseString = "ue_PointLightShadowMaps";
            setTexture(baseString + "DepthMap" + '[' + (char)(nrPointLights + '0') + ']', *pLight->getShadowMap()->getDepthMap());
            set(baseString + "FarPlane" + '[' + (char)(nrPointLights + '0') + ']', pLight->getShadowMap()->camera.renderDistance);
        }
    }

    else {
        set(uniformNameLight + ".isShadowMapped", (int)0);
    }

}

void Environment::bind(Shader* shader)
{
    shader->bind();

    for(auto it = shader->uniformNames.begin(); it != shader->uniformNames.end(); it++)
    {
        if((*it)[0] == 'u' && (*it)[1] == 'e' && (*it)[2] == '_')
        {
            bool isTexture = false;
            for(auto const& iteratorUniformMap : shaderUniformMap.textureMap)
            {
                if(*it == iteratorUniformMap.first)
                {
                    //cout << "Environment texture: " << *it << " slot: " << shader->textureUniformCounter << endl;
                    RV_ASSERT(shader != &RenderManager::get()->directionalShadowMapShader, ""); // temp, can be removed
                    RV_ASSERT(shader != &RenderManager::get()->omnidirectionalShadowMapShader, ""); // temp, can be removed

                    RV_ASSERT(iteratorUniformMap.second->isInited == true, "setting a environment texture that is not inited");

                    shaderUniformMap.set(*it, (int)(shader->textureUniformCounter));
                    iteratorUniformMap.second->bind(shader->textureUniformCounter);

                    shader->textureUniformCounter++;
                    isTexture = true;
                    break;
                }
            }

            if(isTexture == false)
            {
                if(shaderUniformMap.map.find(*it) != shaderUniformMap.map.end()) // can be optimized. doing map.find() twice, once here, and second time in shaderUniformMap.uploadUniform()
                {
                    shaderUniformMap.uploadUniform(*shader, *it);
                }
            }
        }
    }
}