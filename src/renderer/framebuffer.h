#pragma once

#include <glad/glad.h>

struct FramebufferSpecification
{
	unsigned int width, height;
	unsigned int Samples = 1;

	bool swapChainTarget = false;
};

class Framebuffer 
{
public:
	Framebuffer();
	~Framebuffer();
	void setUp(FramebufferSpecification& specification1);
	void invalidate();
	void bind();
	void unbind();
	unsigned int ID;
	unsigned int colorAttachment, depthAttachment;
	FramebufferSpecification specification;
};