#include "scene.h"

#include <iostream>

#include "engine.h"

Scene::Scene(int width, int height){
	projection = glm::mat4(1);
	cameras.push_back(Camera());
	cameras[0].setPosition(glm::vec3(0, 0, -5));
	cameras[0].lookAt(glm::vec3(0, 0, 0));
}

void Scene::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	projection = glm::perspective(glm::radians(45.0f), (float)Engine::getInstance()->getWidth() / (float)Engine::getInstance()->getHeight(), 0.1f, 100.0f);
	for(auto it = objects.begin(); it != objects.end(); ++it){
		(*it)->renderer.draw((*it)->getModel(), cameras[0].getViewMatrix(), projection, cameras[0], lights);
	}
}
