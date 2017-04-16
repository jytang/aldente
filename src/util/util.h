#ifndef _UTIL_H_
#define _UTIL_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Util
{
    public:
		static GLuint quadVAO;
		static GLuint quadVBO;

        static void print_vec3(glm::vec3 v);
        static void print_mat4(glm::mat4 m);
		static unsigned char * loadPPM(const char * filename, int & width, int & height);
        static glm::mat4 calc_bezier_mat(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
        static glm::vec3 trackball_position(double x_pos, double y_pos, int width, int height);
		static void render_quad();
		static void seed(unsigned int s);
		static float random(float min, float max);
		static bool within_rect(glm::vec2 pos, glm::vec2 bottom_left, glm::vec2 top_right);
};

#endif
