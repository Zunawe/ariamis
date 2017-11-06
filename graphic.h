#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <time.h>

#ifndef OPENGL_HEADERS
#define OPENGL_HEADERS
#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#endif

#include "shader.h"
#include "shader_program.h"
#include "mesh.h"
#include "texture.h"

using namespace std;

GLFWwindow *window;

vector<glm::vec3> vertices;
Mesh *mesh;
Texture *texture;

unsigned int VBO;
unsigned int EBO;
unsigned int VAO;

ShaderProgram shader_program;