#include"environment.h"
#include"render_manager.h"

void Environment::setLights()
{
    int nrPointLights = 0;
    int nrDirectionalLights = 0;

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->valid)
        {
            if(itEntity->has<PointLightComponent>())
            {
                RV_ASSERT(nrPointLights < 10, ""); // can be implemented, just string and one digit stuff
                auto* pLight = &itEntity->get<PointLightComponent>()->light;

                if(pLight->on == false)
                {
                    RV_ASSERT(false, "");
                    continue;
                }

                std::string uniformNameLight = "ue_PointLights[";
                uniformNameLight += (char)(nrPointLights + '0');
                uniformNameLight += "]";

                set(uniformNameLight + ".position", itEntity->get<TransformComponent>()->position);

                set(uniformNameLight + ".ambient", pLight->ambient);
                set(uniformNameLight + ".diffuse", pLight->diffuse);
                set(uniformNameLight + ".specular", pLight->specular);

                set(uniformNameLight + ".constant", pLight->constant);
                set(uniformNameLight + ".linear", pLight->linear);
                set(uniformNameLight + ".quadratic", pLight->quadratic);

                nrPointLights++;
            }
            if(itEntity->has<DirectionalLightComponent>())
            {
                RV_ASSERT(nrDirectionalLights < 10, ""); // can be implemented, just string and one digit stuff

                auto* pLight = &itEntity->get<DirectionalLightComponent>()->light;

                if(pLight->on == false)
                {
                    RV_ASSERT(false, "");
                    continue;
                }

                std::string uniformNameLight = "ue_DirectionalLights[";
                uniformNameLight += (char)(nrDirectionalLights + '0');
                uniformNameLight += "]";

                set(uniformNameLight + ".direction", itEntity->get<DirectionalLightComponent>()->light.direction);

                set(uniformNameLight + ".ambient", pLight->ambient);
                set(uniformNameLight + ".diffuse", pLight->diffuse);
                set(uniformNameLight + ".specular", pLight->specular);

                if(pLight->isShadowMapped)
                {
                    setTexture(uniformNameLight + ".shadowMap.depthMap", pLight->shadowMap.depthMap);

                    Camera tempShadowMapCamera(pLight->shadowMap.nearRenderDistance, pLight->shadowMap.renderDistance, 90.f);
                    tempShadowMapCamera.setViewMatrix(itEntity->get<TransformComponent>()->position, itEntity->get<TransformComponent>()->rotation);
                    tempShadowMapCamera.setOrthographicProjection(pLight->shadowMap.width);

                    Mat4 viewProjectionMatrix = multiply(tempShadowMapCamera.projectionMatrix, tempShadowMapCamera.viewMatrix);
                    set(uniformNameLight + ".shadowMap.viewProjectionMatrix", viewProjectionMatrix);

                    RV_ASSERT(pLight->shadowMap.nearRenderDistance != 0 && pLight->shadowMap.renderDistance != 0 && pLight->shadowMap.nearRenderDistance < pLight->shadowMap.renderDistance && pLight->shadowMap.width != 0, "shadow map is incorrectly or incompletely configured");
                }

                nrDirectionalLights++;
            }
        }
    }

    set("ue_NumberOfPointLights", (int)nrPointLights);
    set("ue_NumberOfDirectionalLights", (int)nrDirectionalLights);
}

void Environment::bind(Shader* shader)
{
    shader->bind();
    shader->environmentTextureUniformCounter = 0;

    for(auto it = shader->uniformNames.begin(); it != shader->uniformNames.end(); it++)
    {
        if((*it)[0] == 'u' && (*it)[1] == 'e' && (*it)[2] == '_')
        {
            bool isTexture = false;
            for(auto const& iteratorUniformMap : shaderUniformMap.textureMap)
            {
                if(*it == iteratorUniformMap.first)
                {
                    cout << "Environment texture: " << *it << " slot: " << shader->environmentTextureUniformCounter << endl;
                    RV_ASSERT(shader != &RenderManager::getInstance()->shadowMapShader, "");

                    shaderUniformMap.set(*it, (int)(shader->environmentTextureUniformCounter));
                    iteratorUniformMap.second->bind(shader->environmentTextureUniformCounter);

                    shader->environmentTextureUniformCounter++;
                    isTexture = true;
                    break;
                }
            }

            if(isTexture == false)
                shaderUniformMap.uploadUniform(*shader, *it);
        }
    }
}