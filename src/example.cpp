#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>
#include "glm/ext.hpp"

#include "engine.h"
#include "scene.h"
#include "obj_loader.h"
#include "light.h"

int main(){
	Engine::createWindow(1024, 1024, "Example");

	Scene s;

	Shader toonShader;
	toonShader.loadSources("data/shaders/default.vs", "data/shaders/default.fs");

	// Objects
	std::shared_ptr<Object> sphere(new Object());
	sphere->renderer.setMesh(loadMeshFromObj("data/objects/sphere.obj"));
	sphere->renderer.setShader(toonShader);

	sphere->update = [](std::shared_ptr<Object> me){
		me->position.x = 6.0f * cos(Engine::getTime());
		me->position.z = 6.0f * sin(Engine::getTime());
	};

	s.objects.push_back(sphere);

	std::shared_ptr<Object> plane(new Object());
	plane->renderer.setMesh(loadMeshFromObj("data/objects/cube.obj"));
	plane->renderer.setShader(toonShader);
	s.objects.push_back(plane);

	plane->translate(glm::vec3(0, -1, 0));
	plane->scale(glm::vec3(50.0f, 0.1f, 50.0f));

	// Lights
	std::shared_ptr<PointLight> pointLight(new PointLight());
	pointLight->position = glm::vec4(1, 0, -2, 1);
	pointLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	pointLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->kc = 1.0f;
	pointLight->kl = 0.05f;
	pointLight->kq = 0.001f;
	s.lights.push_back(pointLight);

	// Input
	float speedMultiplier = 2.0f;
	Camera &camera = s.cameras[0];
	Engine::registerKeyEvent(GLFW_KEY_ESCAPE, [](float dt){
		Engine::quit();
	});
	Engine::registerKeyEvent(GLFW_KEY_W, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, 0, 1), dt * speedMultiplier);
	});
	Engine::registerKeyEvent(GLFW_KEY_S, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, 0, -1), dt * speedMultiplier);
	});
	Engine::registerKeyEvent(GLFW_KEY_A, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(-1, 0, 0), dt * speedMultiplier);
	});
	Engine::registerKeyEvent(GLFW_KEY_D, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(1, 0, 0), dt * speedMultiplier);
	});
	Engine::registerKeyEvent(GLFW_KEY_SPACE, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, 1, 0), dt * speedMultiplier);
	});
	Engine::registerKeyEvent(GLFW_KEY_LEFT_SHIFT, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, -1, 0), dt * speedMultiplier);
	});

	// Need to set yaw and pitch based on the original camera position
	float yaw = 90.0f;
	float pitch = 0.0f;
	double lastX, lastY;
	float sensitivity = 0.1f;
	glfwGetCursorPos(Engine::getWindow(), &lastX, &lastY);
	Engine::registerMouseMoveEvent([&yaw, &pitch, &lastX, &lastY, &camera, sensitivity](double x, double y){
		double dx = (x - lastX) * sensitivity;
		double dy = (y - lastY) * sensitivity;

		lastX = x;
		lastY = y;

		yaw += dx;
		pitch -= dy;

		yaw = yaw > 360.0f ? yaw - 360.0f : (yaw < 0.0 ? yaw + 360.0f : yaw);
		pitch = pitch > 89.0f ? 89.0f : (pitch < -89.0f ? -89.0f : pitch);

		glm::vec3 cameraForward;
		cameraForward.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		cameraForward.y = sin(glm::radians(pitch));
		cameraForward.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		camera.setForward(cameraForward);
	});

	Engine::playScene(s);
}
