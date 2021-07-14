#include<reviv.h>

#include<iostream>

Entity* player;
Entity* camera;
Entity* stanic;

class Sandbox : public Application
{
public:
    Sandbox()
    {

    }

    ~Sandbox()
    {

    }

    void init() override
    {
        cout << "sandbox init()" << endl;;
        camera = Scene::setCameraEntity(Scene::createEntity("Camera"));
        player = Scene::setPlayerEntity(Scene::createEntity("Player"));

        camera->add<PositionComponent>();
        camera->add<RotationComponent>();
        camera->add<CameraComponent>();

        player->add<PositionComponent>();
        player->add<RotationComponent>();

        stanic = Scene::createEntity("Stanic");
        stanic->add<PositionComponent>();
        stanic->add<TransformComponent>();
        stanic->add<VaoComponent>("../../sandbox/assets/models/cube.obj"); ///////
        //stanic->add<MaterialComponent>();
        stanic->add<MaterialComponent>(&RenderManager::getInstance()->shaderTexture);

        //praviMono.setUp("../resources/shaders/monocolor.vs", "../resources/shaders/monocolor.fs");
        //Shader* textureShader = new Shader; // TODO, memory leak
        //textureShader->setUp("../resources/shaders/texture.vs", "../resources/shaders/texture.fs");
        //auto* stanicMat = &stanic->add<MaterialComponent>()->material;
        //*stanicMat = MaterialComponent(Material(*textureShader));

        //stanicTexture.setUp("../resources/textures/stene.png");
        //stanicTexture.bind(0);
        //beloTexture.setUp("../resources/textures/belo.png");

    }

    void onUpdate() override
    {
        cout << "sandbox onUpdate()" << endl;

        stanic->get<PositionComponent>()->position = add(player->get<PositionComponent>()->position, Vec3f(5 * sin(glfwGetTime() * 5), 0, 5 * cos(glfwGetTime() * 5)));
        //*stanicPos = add(*playerPos, Vec3f(5 * sin(glfwGetTime()), 0, 5 * cos(glfwGetTime())));
    }

};

Application* createApplication()
{
    return new Sandbox();
}