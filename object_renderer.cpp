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
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(6 * sizeof(float)));   // Tangent
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(9 * sizeof(float)));   // Color
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, mesh.ATTRIBUTE_SIZE * sizeof(float), (void *)(12 * sizeof(float)));  // Texture
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
	glBindVertexArray(0);

	setShader(Shader::DEFAULT_SHADER);
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
void ObjectRenderer::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const Camera &camera, const std::vector<std::shared_ptr<Light>> &lights){
	shader.use();

	for(int i = 0; i < lights.size(); ++i){
		lights[i]->setUniforms(shader, "lights[" + std::to_string(i) + "]");
	}

	glm::vec3 cameraPos(camera.getPosition());
	shader.setUniform("cameraPos", cameraPos);

	glm::mat3 normalModel(glm::transpose(glm::inverse(model)));
	shader.setUniform("model", model);
	shader.setUniform("normalModel", normalModel);
	shader.setUniform("view", view);
	shader.setUniform("projection", projection);

	glm::mat4 modelViewProjection = projection * view * model;
	shader.setUniform("modelViewProjection", modelViewProjection);

	std::vector<unsigned int> submeshes = mesh.getSubmeshBounds();
	unsigned int submeshStart, submeshNumTriangles;
	glBindVertexArray(VAO);
		for(unsigned int i = 0; i < submeshes.size(); ++i){
			Material &material = materials[materialIndices[i]];

			shader.setUniform("material.ambient", material.ambient);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material.diffuseMap.getID());
			shader.setUniform("material.diffuseMap", 0);
			shader.setUniform("material.diffuse", material.diffuse);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material.specularMap.getID());
			shader.setUniform("material.specularMap", 1);
			shader.setUniform("material.specular", material.specular);

			shader.setUniform("material.shininess", material.shininess);

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

/**
 * Returns the OpenGL-provided ID of the VAO for this renderer.
 * 
 * @return the ID of the VAO for this renderer.
 */
unsigned int ObjectRenderer::getVAO(){
	return this->VAO;
}

/**
 * Returns a reference to the mesh for this renderer. If the mesh is modified,
 * reloadMesh must be called before the modified data is copied back into the GPU's
 * buffers.
 * 
 * @return a reference to the mesh for this renderer.
 */
Mesh& ObjectRenderer::getMesh(){
	return this->mesh;
}

/**
 * Sets the mesh of the object renderer and loads the data into the buffers.
 * 
 * @param mesh the new mesh to be used in this renderer.
 */
void ObjectRenderer::setMesh(const Mesh &mesh){
	this->mesh = mesh;

	this->materialIndices.clear();
	this->materialIndices.resize(this->mesh.getNumSubmeshes());

	reloadMesh();
}

/**
 * Returns a reference to the first material for this renderer.
 * 
 * @return a reference to the first material for this renderer.
 */
Material& ObjectRenderer::getMaterial(){
	return materials[0];
}

/**
 * Returns a reference to the material for a specified submesh.
 * 
 * @param submeshIndex the index of the submesh to get the material for.
 * @return a reference to the material.
 */
Material& ObjectRenderer::getMaterial(unsigned int submeshIndex){
	return materials[materialIndices[submeshIndex]];
}

/**
 * Sets the material for all submeshes in this renderer's mesh.
 * 
 * @param material the new material.
 */
void ObjectRenderer::setMaterial(const Material &material){
	this->materials.clear();
	this->materials.push_back(material);

	this->materialIndices.clear();
	this->materialIndices.resize(mesh.getNumSubmeshes());
	for(int i = 0; i < this->mesh.getNumSubmeshes(); ++i){
		materialIndices.push_back(0);
	}
}

/**
 * Sets the material for the specified submesh in this renderer's mesh.
 * 
 * @param submeshIndex the index of the submesh to set the material for.
 * @param material the new material.
 */
void ObjectRenderer::setMaterial(unsigned int submeshIndex, const Material &material){
	auto materialPosition = std::find(materials.begin(), materials.end(), material);
	if(materialPosition == materials.end()){
		materials.push_back(material);
		materialPosition = materials.end() - 1;
	}

	materialIndices[submeshIndex] = std::distance(materials.begin(), materialPosition);
}

/**
 * Sets the shader for this renderer. By default it is Shader::DEFAULT_SHADER.
 * 
 * @param shader the new shader to use.
 */
void ObjectRenderer::setShader(const Shader &shader){
	this->shader = shader;
}

/**
 * Checks for an OpenGL error and prints the error code and location.
 * 
 * @param location a string for use in identifying where in the code the error occurs.
 */
void ObjectRenderer::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		std::cout << "Error at " << location << ": " << err << std::endl;
	}
}
