#include "obj_loader.hpp"

using namespace std;

ObjectRenderer loadObj(const char *filepath){
	ObjectRenderer renderer;

	vector<Material> materials;
	vector<unsigned int> materialIndices;
	renderer.setMesh(loadMeshFromObj(filepath, materialIndices, materials));
	for(unsigned int i = 0; i < materialIndices.size(); ++i){
		renderer.setMaterial(i, materials[materialIndices[i]]);
	}

	return renderer;
}

Mesh loadMeshFromObj(const char *filepath){
	vector<unsigned int> throwaway;
	vector<Material> throwaway2;
	return loadMeshFromObj(filepath, throwaway, throwaway2);
}

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
						sscanf((*it).c_str(), "%d/%d/%d", indices, indices + 1, indices + 2);

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
					sscanf(line.c_str(), "K%*[ads] %f %f %f", &r, &g, &b);
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
			case 'N':
				{
					float value;
					sscanf(line.c_str(), "N%*[s] %f", &value);

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

vector<string> split(const string &s, char delim){
	vector<string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}


void trim(std::string &s){
   size_t p = s.find_first_not_of(" \t");
   s.erase(0, p);

   p = s.find_last_not_of(" \t");
   if (string::npos != p)
      s.erase(p+1);
}
