#include <ariamis/scene.h>

#include <glm/glm.hpp>

#include <ariamis/engine.h>

Scene::Scene(){
	camera.setForward(glm::vec3(0, 0, 1));
}

/**
 * Runs before every frame.
 */
void Scene::update(){
	for(auto it = objects.begin(); it != objects.end(); ++it){
		(*it)->update();
	}
}

/**
 * Tells this scene's objects to draw themselves.
 */
void Scene::draw(){
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Engine::getWidth() / (float)Engine::getHeight(), 0.1f, 100.0f);
	for(auto it = objects.begin(); it != objects.end(); ++it){
		(*it)->renderer.draw((*it)->getModel(), camera.getViewMatrix(), projection);
	}
}
