#pragma once

#include"window.h"
#include"core/mat.h"
#include"renderer/render_command.h"
#include"renderer/texture.h"

#include"scene/components.h"
#include"scene/scene.h"

#include"renderer/material.h"

class RenderManager {
public:
	RenderManager();

	int init(int windowWidth1, int windowHeight1); ///npr, 800x600, 1920x1080
	void render();
	void shutdown();
	Window window;
	float renderDistance = 1000;

    Shader praviMono;
	bool renderHitbox;
	float* voxelBuffer;
	size_t sizeOfVoxelBuffer;
	bool fullscreen = false;

	void submit(Material* material, const Mat4& transform, const Vao& vao);

	void beginScene();
	void endScene();

	Texture2D stanicTexture;
	Texture2D beloTexture;

	int nrOfValidCubes;

private:
};