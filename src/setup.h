#pragma once

/*
	Setup for low-level systems are done here.
	Including OpenGL, glfw, callbacks, and shaders. 
*/
class Setup {

private:
public:
    static void setup_callbacks();

    static void setup_opengl();

    static void setup_shaders();
};

