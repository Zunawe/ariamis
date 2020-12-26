#include "scene.h"
#include "engine.h"

Scene::Scene(){
	projection = glm::mat4(1);
	cameras.push_back(Camera());
	cameras[0].setForward(glm::vec3(0, 0, 1));
}

/**
 * Runs before every frame.
 */
void Scene::update(){
	for(auto it = objects.begin(); it != objects.end(); ++it){
		(*it)->update(*it);
	}
}

/**
 * Tells this scene's objects to draw themselves.
 */
void Scene::draw(){
	projection = glm::perspective(glm::radians(45.0f), (float)Engine::getWidth() / (float)Engine::getHeight(), 0.1f, 100.0f);
	for(auto it = objects.begin(); it != objects.end(); ++it){
		(*it)->renderer.draw((*it)->getModel(), cameras[0].getViewMatrix(), projection);
	}
}
