#!/bin/bash

cmake -S ../ -B ../build/

# Some cmake flags:
#   GLFW_USE_OSMESA - Use OSMesa for offscreen context creation
#   GLFW_USE_WAYLAND - Use wayland for window creation
