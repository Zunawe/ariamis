#include "object_renderer.hpp"

ObjectRenderer::ObjectRenderer(){
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)0);                     // Vertex
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(3 * sizeof(float)));   // Normal
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(6 * sizeof(float)));   // Color
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(9 * sizeof(float)));   // Texture
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
	glBindVertexArray(0);

	setShader(ShaderProgram::DEFAULT_SHADER);
	setMaterial(Material::DEFAULT_MATERIAL);

	checkErrorAt("Object Renderer Initialization");
}

/**
 * Draws this object. Assumes it is only called within some sort of display function.
 * 
 * @param model the model matrix at the time of drawing the object.
 * @param view the view matrix at the time of drawing the object.
 * @param projection the projection matrix at the time of drawing the object.
 * @param camera the Camera the object is being viewed by.
 */
void ObjectRenderer::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Camera &camera){
	Light light;
	light.pos = glm::vec3(0, 2, 0);
	light.ambient = glm::vec3(0.7f, 0.7f, 0.7f);
	light.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
	light.specular = glm::vec3(0.7f, 0.7f, 0.7f);

	glm::vec3 cameraPos(camera.getPosition());

	shader.use();

	glm::mat3 normalModel(glm::transpose(glm::inverse(model)));
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(shader.getID(), "normalModel"), 1, GL_FALSE, &normalModel[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_FALSE, &projection[0][0]);

	glm::mat4 coordinateTransform = projection * view * model;
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "coordinateTransform"), 1, GL_FALSE, &coordinateTransform[0][0]);

	glBindTexture(GL_TEXTURE_2D, texture.getID());
	glUniform1i(glGetUniformLocation(shader.getID(), "texture0"), 0);

	glUniform3fv(glGetUniformLocation(shader.getID(), "light.pos"), 1, &light.pos[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "light.ambient"), 1, &light.ambient[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "light.diffuse"), 1, &light.diffuse[0]);
	glUniform3fv(glGetUniformLocation(shader.getID(), "light.specular"), 1, &light.specular[0]);

	glUniform3fv(glGetUniformLocation(shader.getID(), "cameraPos"), 1, &cameraPos[0]);

	std::vector<unsigned int> submeshes = mesh.getSubmeshBounds();
	unsigned int submeshStart, submeshNumTriangles;
	glBindVertexArray(VAO);
		for(unsigned int i = 0; i < submeshes.size(); ++i){
			glUniform3fv(glGetUniformLocation(shader.getID(), "material.ambient"), 1, &materials[materialIndices[i]].ambient[0]);
			glUniform3fv(glGetUniformLocation(shader.getID(), "material.diffuse"), 1, &materials[materialIndices[i]].diffuse[0]);
			glUniform3fv(glGetUniformLocation(shader.getID(), "material.specular"), 1, &materials[materialIndices[i]].specular[0]);
			glUniform1f(glGetUniformLocation(shader.getID(), "material.shininess"), materials[materialIndices[i]].shininess);

			submeshStart = submeshes[i];
			submeshNumTriangles = (i + 1 < submeshes.size() ? submeshes[i + 1] : mesh.getNumTriangles()) - submeshStart;
			glDrawElements(GL_TRIANGLES, submeshNumTriangles * 3, GL_UNSIGNED_INT, (GLvoid *)(submeshStart * 3 * sizeof(GLuint)));
		}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	checkErrorAt("ObjectRenderer draw");
}

/**
 * Copies the current mesh into the buffer. If this object's currently set mesh object is changed after it is set,
 * this function must be called before changes take effect. (Note: Using setMesh to set the mesh to a new Mesh object
 * calls this method by default.)
 */
void ObjectRenderer::reloadMesh(){
	glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.getNumVertices() * mesh.ATTRIBUTE_SIZE, mesh.getVertexData(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.getNumTriangles() * 3, mesh.getIndexData(), GL_STATIC_DRAW);
	glBindVertexArray(0);
}

unsigned int ObjectRenderer::getVAO(){
	return this->VAO;
}

Mesh* ObjectRenderer::getMesh(){
	return &(this->mesh);
}

/**
 * Sets the mesh of the object renderer and loads the data into the buffers.
 */
void ObjectRenderer::setMesh(const Mesh &mesh){
	this->mesh = mesh;

	this->materialIndices.clear();
	this->materialIndices.resize(this->mesh.getNumSubmeshes());

	reloadMesh();
}

void ObjectRenderer::setMaterial(const Material &material){
	this->materials.clear();
	this->materials.push_back(material);

	this->materialIndices.clear();
	this->materialIndices.resize(mesh.getNumSubmeshes());
	for(int i = 0; i < this->mesh.getNumSubmeshes(); ++i){
		materialIndices.push_back(0);
	}
}

void ObjectRenderer::setMaterial(unsigned int submeshIndex, const Material &material){
	auto materialPosition = std::find(materials.begin(), materials.end(), material);
	if(materialPosition == materials.end()){
		materials.push_back(material);
		materialPosition = materials.end() - 1;
	}

	materialIndices[submeshIndex] = std::distance(materials.begin(), materialPosition);
}

void ObjectRenderer::setTexture(const Texture &texture){
	this->texture = texture;
}

void ObjectRenderer::setShader(const ShaderProgram &shader){
	this->shader = shader;
}

void ObjectRenderer::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		std::cout << "Error at " << location << ": " << err << std::endl;
	}
}