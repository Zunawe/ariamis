#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "engine.h"
#include "scene.h"
#include "obj_loader.h"
#include "light.h"

int main(){
	Engine ariamis = *(Engine::getInstance());
	ariamis.createWindow(1024, 1024, "Example");

	Scene s(ariamis.getWidth(), ariamis.getHeight());

	std::shared_ptr<Object> sphere(new Object());
	sphere->renderer.setMesh(loadMeshFromObj("data/objects/sphere.obj"));
	s.objects.push_back(sphere);

	std::shared_ptr<PointLight> pointLight(new PointLight());
	pointLight->position = glm::vec4(1, 0, -2, 1);
	pointLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->kc = 1.0f;
	pointLight->kl = 0.05f;
	pointLight->kq = 0.001f;
	s.lights.push_back(pointLight);

	float speedMultiplier = 2.0f;
	ariamis.registerKeyEvent(GLFW_KEY_W, [&s, speedMultiplier](float dt){
		Camera &camera = s.cameras[0];
		camera.moveRelative(glm::vec3(0, 0, 1), dt * speedMultiplier);
	});
	ariamis.registerKeyEvent(GLFW_KEY_S, [&s, speedMultiplier](float dt){
		Camera &camera = s.cameras[0];
		camera.moveRelative(glm::vec3(0, 0, -1), dt * speedMultiplier);
	});
	ariamis.registerKeyEvent(GLFW_KEY_A, [&s, speedMultiplier](float dt){
		Camera &camera = s.cameras[0];
		camera.moveRelative(glm::vec3(-1, 0, 0), dt * speedMultiplier);
	});
	ariamis.registerKeyEvent(GLFW_KEY_D, [&s, speedMultiplier](float dt){
		Camera &camera = s.cameras[0];
		camera.moveRelative(glm::vec3(1, 0, 0), dt * speedMultiplier);
	});
	ariamis.registerKeyEvent(GLFW_KEY_SPACE, [&s, speedMultiplier](float dt){
		Camera &camera = s.cameras[0];
		camera.moveRelative(glm::vec3(0, 1, 0), dt * speedMultiplier);
	});
	ariamis.registerKeyEvent(GLFW_KEY_LEFT_SHIFT, [&s, speedMultiplier](float dt){
		Camera &camera = s.cameras[0];
		camera.moveRelative(glm::vec3(0, -1, 0), dt * speedMultiplier);
	});

	ariamis.playScene(s);
}
