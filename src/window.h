#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_INCLUDE_GLEXT

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Window
{
    public:
        static GLFWwindow* create_window(int width, int height, const char *window_title);
};

#endif
