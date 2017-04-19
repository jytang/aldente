#pragma once

#include <GL/glew.h>
#include "geometry.h"

#include <vector>
#include <time.h>

#include "util/util.h"

#define NONE 0
#define GRASS 1
#define SAND 2
#define ROCK 3
#define WATER 4
#define SAND_TWO 5
#define SNOW 6
#define OBSIDIAN 7
#define SPACE 8
#define STONE 9

class GeometryGenerator
{
public:
	static std::vector<Geometry *> geometries;

	static void clean_up();

	static Geometry *generate_cube(GLfloat scale, bool with_normals);
	static Geometry *generate_sphere(GLfloat radius, GLuint divisions);
	static Geometry *generate_cylinder(GLfloat radius, GLfloat height, GLuint divisions, bool is_centered);
	static Geometry *generate_plane(GLfloat scale, int texture_type);
	static Geometry *generate_bezier_plane(GLfloat radius, GLuint num_curves, GLuint segmentation, GLfloat waviness, int texture_type, unsigned int seed);
};