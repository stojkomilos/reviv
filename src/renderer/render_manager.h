#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb_image/stb_image.h>

#include"core/mat.h"
//#include"renderer/shader.h"
//#include"physics/physics_manager.h"
//#include"renderer/modelloader.h"
//#include"physics/physics_sim.h"
//#include"renderer/buffer.h"
//#include"renderer/vertex_array.h"
#include"renderer/render_command.h"
//#include"renderer/camera.h"
#include"renderer/texture.h"
//#include"renderer/framebuffer.h"

#include"scene/components.h"
#include"scene/scene.h"

#include"renderer/material.h"

class RenderManager {
public:
	int startUp(int windowWidth1, int windowHeight1); ///npr, 800x600, 1920x1080
	int render();
	int shutDown();
//	VertexArray cubeVao, sphereVao;
//	VertexBuffer cubeVbo, sphereVbo;
	GLFWwindow* window;
	int windowWidth, windowHeight;
	float renderDistance = 1000;
    Shader praviMono;
	bool renderHitbox;
	float* voxelBuffer;
	size_t sizeOfVoxelBuffer;
	bool fullscreen = false;

	static void openGlLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParams);

	//void submit(Shader& shader, Vao& object, Mat4& transform);
	void submit(Material* material, const Mat4& transform, const Vao& vao);
	void beginScene();
	void endScene();

	Texture2D stanicTexture;
	Texture2D beloTexture;

	int nrOfValidCubes;

	RenderManager();
private:
};