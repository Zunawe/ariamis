#include "object_renderer.hpp"

ObjectRenderer::ObjectRenderer(){
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	checkErrorAt("Object Renderer Constructor");
}

void ObjectRenderer::draw(const glm::mat4 *model, const glm::mat4 *view, const glm::mat4 *projection){
	shader->use();
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "model"), 1, GL_FALSE, &(*model)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "view"), 1, GL_FALSE, &(*view)[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, GL_FALSE, &(*projection)[0][0]);
	glUniform1i(glGetUniformLocation(shader->getID(), "texture1"), 0);
	glBindTexture(GL_TEXTURE_2D, texture->getID());
	glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->getNumVertices() * 8, mesh->getVertexData(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->getNumTriangles() * 3, mesh->getIndexData(), GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, mesh->getNumTriangles() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int ObjectRenderer::getVAO(){
	return this->VAO;
}

void ObjectRenderer::setMesh(Mesh *mesh){
	this->mesh = mesh;
}

void ObjectRenderer::setTexture(Texture *texture){
	this->texture = texture;
}

void ObjectRenderer::setShader(ShaderProgram *shader){
	this->shader = shader;
	checkErrorAt("Setting shader");	
}

void ObjectRenderer::checkErrorAt(const char *location){
	GLenum err;
	if((err = glGetError()) != GL_NO_ERROR){
		std::cout << "Error at " << location << ": " << (err == GL_INVALID_VALUE) << std::endl;
	}
}