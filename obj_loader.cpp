#include "obj_loader.hpp"

using namespace std;

Mesh loadMeshFromObj(const char *filepath){
	ifstream file;
	string line;

	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> uv;

	Mesh mesh;

	file.open(filepath);


	while(getline(file, line)){
		vector<string> tokens = split(line, ' ');

		if(!tokens.size() || !tokens[0].length() || tokens[0][0] == '#'){
			continue;
		}
		else if(tokens[0] == "v"){
			vertices.push_back(glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		}
		else if(tokens[0] == "vt"){
			uv.push_back(glm::vec2(stof(tokens[1]), stof(tokens[2])));
		}
		else if(tokens[0] == "vn"){
			normals.push_back(glm::vec3(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
		}
		else if(tokens[0] == "f"){
			vector<int>	faceIndices;

			for(unsigned int i = 1; i < tokens.size(); ++i){
				vector<string> splitToken = split(tokens[i], '/');
				vector<int> indices;

				for(unsigned int j = 0; j < splitToken.size(); ++j){
					int index = splitToken[j].length() > 0 ? stoi(splitToken[j]) : 0;

					if(index < 0){
						switch(j){
							case 0:
								index += vertices.size();
								break;
							case 1:
								index += uv.size();
								break;
							case 2:
								index += normals.size();
								break;
						}
					}
					else{
						--index;
					}

					indices.push_back(index);
				}

				faceIndices.push_back(mesh.addVertex(vertices[indices[0]]));
				if(indices.size() == 2){
					mesh.setTextureCoordinate(uv[indices[1]]);
				}
				else if(indices.size() == 3){
					if(indices[1] >= 0){
						mesh.setTextureCoordinate(uv[indices[1]]);
					}
					mesh.setNormal(normals[indices[2]]);
				}
			}

			for(unsigned int i = 2; i < faceIndices.size(); ++i){
				mesh.addTriangle(faceIndices[0], faceIndices[i - 1], faceIndices[i]);
			}
		}
	}

	file.close();

	return mesh;
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