#include"material.h"

/*
Da bi submitovao treba ti:
 - Transform
 - Material
    - Texture bind
    - Shader bind
    - Uniform upload (object and globa/scene)
            shaderTexture.uploadUniform3f("u_LightPosition", gPointLightPosition);
            shaderTexture.uploadUniform3f("u_CameraPosition", camera.position);
            shaderTexture.uploadUniform1i("u_Texture", 1);
*/

// shader.bind()
// shader.uploadUniform()                       // mozda ne mora da upload svaki put?
// shader.uploadUniform("u_Texture", int?)
// submit()



//Mat4 model;

//gRenderCommand.setClearColor(Vec4f(0.0f, 0.0f, 0.0f, 0.9f));
//gRenderCommand.clear();

//beginScene(*gCamera.getComponent<PerspectiveCameraComponent>());

//gCamera.getComponent<PerspectiveCameraComponent>()->alignWithEntity(gPlayer);

//auto playerPosition = (*gPlayer.getComponent<PositionComponent>()).position;

//cout << "gPlayer->position=";
//log(playerPosition);

//model = translate(identity, playerPosition);

//shaderMonoChroma.bind();
//shaderMonoChroma.uploadUniform4f("u_Color", Vec4f(1, 1, 1, 1));

//stanicTexture.bind(0);
//shaderMonoChroma.uploadUniform1i("u_Texture", 0);

//submit(shaderMonoChroma, *gStanic.getComponent<VaoComponent>(), model);
