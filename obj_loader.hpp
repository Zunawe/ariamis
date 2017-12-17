#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "mesh.hpp"

Mesh loadMeshFromObj(const char *filepath);

template<typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

#endif
