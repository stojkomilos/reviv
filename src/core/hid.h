#pragma once

#include<GLFW/glfw3.h>

#include"core/mat.h"
#include"core/entity.h"
#include"core/basic_components.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
