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

    for(auto itEntity = Scene::getEntityList()->begin(); itEntity != Scene::getEntityList()->end(); itEntity++)
    {
        if(itEntity->valid)
        {
            if(itEntity->has<PointLightComponent>()) // set point lights
            {
                RV_ASSERT(false, ""); // temp. can be removed
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

            if(itEntity->has<DirectionalLightComponent>()) // set directional lights
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

                if(uniformNameLight == "ue_DirectionalLight[1]")
                    continue;

                Vec3f direction = getDirectionFromRotation(itEntity->get<TransformComponent>()->rotation);
                set(uniformNameLight + ".direction", direction);

                set(uniformNameLight + ".ambient", pLight->ambient);
                set(uniformNameLight + ".diffuse", pLight->diffuse);
                set(uniformNameLight + ".specular", pLight->specular);

                if(pLight->isShadowMapped)
                {
                    setTexture(uniformNameLight + ".shadowMap.depthMap", pLight->shadowMap.depthMap);

                    Camera* pCamera = &pLight->shadowMap.camera;
                    pCamera->nearRenderDistance = pLight->shadowMap.nearRenderDistance;
                    pCamera->renderDistance = pLight->shadowMap.renderDistance;
                    pCamera->setViewMatrix(itEntity->get<TransformComponent>()->position, itEntity->get<TransformComponent>()->rotation);
                    pCamera->setOrthographicProjection(pLight->shadowMap.width, 1);

                    //cout << "light Position: ";
                    //log(itEntity->get<TransformComponent>()->position);
                    //cout << "light Rotation: ";
                    //log(itEntity->get<TransformComponent>()->rotation);
                    //cout << endl << endl << endl;

                    set(uniformNameLight + ".shadowMap.viewMatrix", pLight->shadowMap.camera.viewMatrix);
                    set(uniformNameLight + ".shadowMap.projectionMatrix", pLight->shadowMap.camera.projectionMatrix);

                    RV_ASSERT(pLight->shadowMap.nearRenderDistance != 0 && pLight->shadowMap.renderDistance != 0 && pLight->shadowMap.nearRenderDistance < pLight->shadowMap.renderDistance && pLight->shadowMap.width != 0, "shadow map is incorrectly or incompletely configured");
                }

                nrDirectionalLights++;
            }
        }
    }

    set("ue_NumberOfPointLights", (int)nrPointLights);
    set("ue_NumberOfDirectionalLights", (int)nrDirectionalLights);

    RV_ASSERT(nrPointLights == 0, ""); // temp, can be removed
    //cout << "nrDirectionalLights: " << nrDirectionalLights << endl;
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
                    //cout << "Environment texture: " << *it << " slot: " << shader->environmentTextureUniformCounter << endl;
                    RV_ASSERT(shader != &RenderManager::getInstance()->shadowMapShader, "");

                    shaderUniformMap.set(*it, (int)(shader->environmentTextureUniformCounter));
                    iteratorUniformMap.second->bind(shader->environmentTextureUniformCounter);

                    shader->environmentTextureUniformCounter++;
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