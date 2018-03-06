#include "scene.h"

#include <iostream>

Scene::Scene(int width, int height){
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	cameras.push_back(Camera());
	cameras[0].setPosition(glm::vec3(0, 0, -5));
	cameras[0].lookAt(glm::vec3(0, 0, 0));
}

void Scene::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(auto it = objects.begin(); it != objects.end(); ++it){
		(*it)->renderer.draw((*it)->getModel(), cameras[0].getViewMatrix(), projection, cameras[0], lights);
	}
}
