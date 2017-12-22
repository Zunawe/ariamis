#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include "mesh.hpp"
#include "object_renderer.hpp"

ObjectRenderer loadObj(const char *filepath);
Mesh loadMeshFromObj(const char *filepath);
Mesh loadMeshFromObj(const char *filepath, std::vector<unsigned int> &materialIndices);
Mesh loadMeshFromObj(const char *filepath, std::vector<unsigned int> &materialIndices, std::vector<Material> &materials);
std::map<std::string, Material> loadMaterialsFromMtl(const char *filepath);

template<typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

#endif
