#ifndef DRAW_H
#define DRAW_H
#include "GLinclude.h"
#include "cube.h"
#include "ball.h"
#include "object.h"
#include "matrix.h"
#include "texture.h"
#include "glsl.h"
#include "kdtree.hpp"
#include <vector>
#include <string>
#include <random>
#define MOVE_SPEED 0.5f
#define ROTATE_SPEED 0.1f


#define SUN_LIGHT GL_LIGHT0

void initTexture();
void initObjects(std::vector<object *> &);
void glInit();
void drawCoordinateString(glm::vec3, glm::vec3, int, int, float, int);
void move(float, float, float, glm::vec3 &, glm::vec3 &);
void update(glm::vec3 &, glm::vec3 &);
void display(GLFWwindow *, int, int, float, int, glm::vec3 &, glm::vec3 &, std::vector<object *> &);
void keyboard(GLFWwindow *, int, int, int, int);
void updatePhysics(float, std::vector<object *> &);

#endif