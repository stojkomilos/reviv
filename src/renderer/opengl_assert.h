#pragma once

#include<iostream>
#include<cassert>
#include<glad/glad.h>


#define GLCall(x) GLClearError();\
	x;\
	assert(GLLogCall())
static bool GLLogCall() {
	while (GLenum error = glGetError()) {

		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
		return false;
	}
	return true;
}
static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}