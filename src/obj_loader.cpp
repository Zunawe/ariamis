#include "obj_loader.h"

using namespace std;

/**
 * Creates a new ObjectRenderer and loads a mesh and the corresponding materials
 * into it from a Wavefront OBJ file (and optional MTL file if the OBJ specifies one).
 * 
 * @param filepath the location of the file to read.
 * @return a newly constructed ObjectRenderer with a mesh and materials loaded from
 * the provided filepath.
 */
ObjectRenderer loadObj(const char *filepath){
	ObjectRenderer renderer;
	loadObj(filepath, renderer);
	return renderer;
}

/**
 * Loads a mesh and corresponding materials into an existing ObjectRenderer from
 * a Wavefront OBJ file (and optional MTL file if the OBJ specifies one).
 * 
 * @param filepath the location of the file to read.
 * @param renderer the renderer to load the mesh into.
 */
void loadObj(const char *filepath, ObjectRenderer &renderer){
	vector<Material> materials;
	vector<unsigned int> materialIndices;
	renderer.setMesh(loadMeshFromObj(filepath, materialIndices, materials));
	for(unsigned int i = 0; i < materialIndices.size(); ++i){
		renderer.setMaterial(i, materials[materialIndices[i]]);
	}
}

/**
 * Loads a mesh from a Wavefront OBJ file. Includes submeshes specified by
 * 'usemtl' lines, but does not save materials.
 * 
 * @param filepath the location of the file to read.
 * @return a newly constructed Mesh object loaded from the provided filepath
 */
Mesh loadMeshFromObj(const char *filepath){
	vector<unsigned int> throwaway;
	vector<Material> throwaway2;
	return loadMeshFromObj(filepath, throwaway, throwaway2);
}

/**
 * Loads a mesh from a Wavefront OBJ file. Includes submeshes specified by
 * 'usemtl' lines as well as indices for materials, but does not save materials.
 * Material indices are for multiple disjoint submeshes with the same material.
 * 
 * @param filepath the location of the file to read.
 * @param materialIndices an output parameter that is cleared and populated with
 * material indices.
 * @return a newly constructed Mesh object loaded from the provided filepath
 */
Mesh loadMeshFromObj(const char *filepath, vector<unsigned int> &materialIndices){
	vector<Material> throwaway;
	return loadMeshFromObj(filepath, materialIndices, throwaway);
}

/**
 * Loads a mesh from a Wavefront OBJ file. Includes submeshes specified by
 * 'usemtl' lines as well as indices for materials and materials themselves (if
 * there is a valid MTL file). Material indices are for multiple disjoint
 * submeshes with the same material.
 * 
 * @param filepath the location of the file to read.
 * @param materialIndices an output parameter that is cleared and populated with
 * material indices.
 * @param materials an output parameter that is cleared and populated with
 * materials. The material indices refer to the order of this list.
 * @return a newly constructed Mesh object loaded from the provided filepath
 */
Mesh loadMeshFromObj(const char *filepath, vector<unsigned int> &materialIndices, vector<Material> &materials){
	ifstream file;
	string line;

	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> uv;
	vector<string> materialNames;
	map<string, Material> usedMaterials;

	Mesh mesh;
	materialIndices.clear();
	materials.clear();

	file.open(filepath);

	while(getline(file, line)){
		if(line.size() == 0)
			continue;

		switch(line.c_str()[0]){
			case '#':
			case 'o':
				continue;
			case 'v':
				{
					float x, y, z;
					switch(line.c_str()[1]){
						case ' ':
							sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
							vertices.push_back(glm::vec3(x, y, z));
							break;
						case 'n':
							sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
							normals.push_back(glm::vec3(x, y, z));
							break;
						case 't':
							sscanf(line.c_str(), "vt %f %f", &x, &y);
							uv.push_back(glm::vec2(x, y));
							break;
						default:
							cout << "Encountered unsupported instruction \"" << line << "\"" << endl;
							break;
					}
				}
				break;
			case 'f':
				{
					vector<string> tokens = split(line, ' ');
					vector<int> faceVertexIndices;

					int indices[3];
					for(auto it = tokens.begin() + 1; it != tokens.end(); ++it){
						indices[0] = indices[1] = indices[2] = 0;
						vector<string> stringIndices = split(*it, '/');
						for(int i = 0; i < stringIndices.size(); ++i){
							if(stringIndices[i] == ""){
								continue;
							}
							indices[i] = stoi(stringIndices[i]);
						}

						faceVertexIndices.push_back(indices[0] > 0 ? mesh.addVertex(vertices[indices[0] - 1]) : mesh.addVertex(vertices[vertices.size() + indices[0]]));
						if(indices[1] != 0){
							indices[1] > 0 ? mesh.setTextureCoordinate(uv[indices[1] - 1]) : mesh.setTextureCoordinate(uv[uv.size() + indices[1]]);
						}
						if(indices[2] != 0){
							indices[2] > 0 ? mesh.setNormal(normals[indices[2] - 1]) : mesh.setNormal(normals[normals.size() + indices[2]]);
						}
					}

					for(unsigned int i = 2; i < faceVertexIndices.size(); ++i){
						mesh.addTriangle(faceVertexIndices[0], faceVertexIndices[i - 1], faceVertexIndices[i]);
					}
				}
				break;
			case 'u':
				{
					mesh.startNewSubmesh();

					char materialNameBuffer[128];
					sscanf(line.c_str(), "usemtl %127s", materialNameBuffer);

					int materialIndex = find(materialNames.begin(), materialNames.end(), materialNameBuffer) - materialNames.begin();
					if(materialIndex == materialNames.size()){
						materialNames.push_back(string(materialNameBuffer));
					}
					materialIndices.push_back(materialIndex);
				}
				break;
			case 'm':
				{
					char mtlPathBuffer[128];
					sscanf(line.c_str(), "mtllib %127s", mtlPathBuffer);

					auto lastSlashPos = string(filepath).rfind('/');
					string mtlPath = (lastSlashPos == string::npos ? "" : string(filepath, lastSlashPos + 1)) + string(mtlPathBuffer);

					map<string, Material> newMaterials = loadMaterialsFromMtl(mtlPath.c_str());
					usedMaterials.insert(newMaterials.begin(), newMaterials.end());
				}
				break;
			default:
				cout << "Encountered unsupported instruction \"" << line << "\"" << endl;
				break;
		}
	}

	for(auto it = materialNames.begin(); it != materialNames.end(); ++it){
		materials.push_back(usedMaterials[*it]);
	}
	if(materials.size() == 0){
		materials.push_back(Material::DEFAULT_MATERIAL);
	}
	materialIndices.resize(mesh.getNumSubmeshes());

	file.close();

	return mesh;
}

/**
 * Loads a set of materials from a Wavefront MTL file.
 * 
 * @param filepath the location of the file to read.
 * @return the loaded materials. The key is the name specified in the MTL file
 * and the value is the corresponding Material object.
 */
map<string, Material> loadMaterialsFromMtl(const char *filepath){
	map<string, Material> materials;

	ifstream file;
	string line;
	string currentMaterialName;

	file.open(filepath);

	while(getline(file, line)){
		trim(line);
		if(line.size() == 0)
			continue;

		switch(line.c_str()[0]){
			case '#':
				continue;
			case 'n':
				{
					char mtlNameBuffer[128];
					sscanf(line.c_str(), "newmtl %s", mtlNameBuffer);

					currentMaterialName = string(mtlNameBuffer);
					materials.insert(pair<string, Material>(currentMaterialName, Material()));
				}
				break;
			case 'K':
				{
					float r, g, b;
					sscanf(line.c_str(), "K%*1c%*[ \t]%f%*[ \t]%f%*[ \t]%f", &r, &g, &b);
					glm::vec3 color(r, g, b);

					switch(line.c_str()[1]){
						case 'a':
							materials[currentMaterialName].ambient = color;
							break;
						case 'd':
							materials[currentMaterialName].diffuse = color;
							break;
						case 's':
							materials[currentMaterialName].specular = color;
							break;
						default:
							cout << "Encountered unsupported instruction \"" << line << "\"" << endl;
							break;
					}
				}
				break;
			case 'N':
				{
					float value;
					sscanf(line.c_str(), "N%*1c%*[ \t]%f", &value);

					switch(line.c_str()[1]){
						case 's':
							materials[currentMaterialName].shininess = value;
							break;
						default:
							cout << "Encountered unsupported instruction \"" << line << "\"" << endl;
							break;
					}
				}
				break;
			default:
				cout << "Encountered unsupported instruction \"" << line << "\"" << endl;
				break;
		}
	}

	return materials;
}

/**
 * Splits a string by a delimiting character.
 * 
 * @param s the string to split.
 * @param delim the delimiting character.
 * @param result an iterable object in which the tokens are stored.
 */
template<typename Out>
void split(const string &s, char delim, Out result){
	if(s.find(delim) == string::npos){
		return;
	}
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)){
        *(result++) = item;
    }
}

/**
 * Splits a string by a delimiting character.
 * 
 * @param s the string to split.
 * @param delim the delimiting character.
 * @return a vector containing the tokens.
 */
vector<string> split(const string &s, char delim){
	vector<string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

/**
 * Removes all leading and trailing spaces and tabs from a string;
 * 
 * @param s the string to modify.
 */
void trim(string &s){
   size_t p = s.find_first_not_of(" \t");
   s.erase(0, p);

   p = s.find_last_not_of(" \t");
   if (string::npos != p)
      s.erase(p+1);
}
