#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image/stb_image.h>

#include"core/mat.h"
#include"renderer/shader.h"
#include"renderer/shader.h"
//#include"physics/physics_manager.h"
#include"renderer/modelloader.h"
//#include"physics/physics_sim.h"
#include"renderer/buffer.h"
#include"renderer/vertex_array.h"
#include"renderer/render_command.h"
#include"renderer/camera.h"
#include"renderer/texture.h"
#include"renderer/framebuffer.h"

#include"core/entity.h"
#include"core/basic_components.h"

class RenderManager {
public:
	RenderManager();
	int startUp(int windowWidth1, int windowHeight1); ///npr, 800x600, 1920x1080
	int render();
	int shutDown();
//	VertexArray cubeVao, sphereVao;
//	VertexBuffer cubeVbo, sphereVbo;
	GLFWwindow* window;
	int windowWidth, windowHeight;
	float renderDistance = 1000;
	Shader shaderTexture;
	Shader shaderNoTexture;
	Shader shaderInstanced;
	Shader shaderMonoChroma;
	bool renderHitbox;
	float* voxelBuffer;
	size_t sizeOfVoxelBuffer;
	bool fullscreen = false;
	
	void submit(Shader& shader, VertexArrayComponent& object, Mat4& transform);
	void beginScene(PerspectiveCameraComponent& camera);
	void endScene();

	Texture2D stanicTexture;
	Texture2D beloTexture;

	//PerspectiveCamera camera;

	int nrOfValidCubes;

	struct SceneData
	{
		Mat4 viewMatrix, projectionMatrix;
	};
	SceneData* sceneData;

private:
};

/// NOVO