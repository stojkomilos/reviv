#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "reviv/core/mat.h"
#include "reviv/renderer/shader.h"
#include "reviv/renderer/shader.h"
#include "reviv/physics/physics_manager.h"
#include "reviv/renderer/modelloader.h"
#include "reviv/physics/physics_sim.h"
#include "reviv/renderer/buffer.h"
#include "reviv/renderer/vertex_array.h"
#include "reviv/renderer/render_command.h"
#include "reviv/renderer/camera.h"
#include "reviv/renderer/texture.h"
#include "reviv/renderer/framebuffer.h"

class RenderManager {
public:
	RenderManager();
	int startUp(int windowWidth1, int windowHeight1); ///npr, 800x600, 1920x1080
	int render();
	int shutDown();
	VertexArray cubeVao, sphereVao;
	VertexBuffer cubeVbo, sphereVbo;
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
	
	void submit(Shader& shader, VertexArray& object, mat::mat4& transform);
	void beginScene(PerspectiveCamera& camera);
	void endScene();

	Texture2D stanicTexture;
	Texture2D beloTexture;

	PerspectiveCamera camera;

	int nrOfValidCubes;

	struct SceneData
	{
		mat::mat4 viewMatrix, projectionMatrix;
	};
	SceneData* sceneData;

private:


};

