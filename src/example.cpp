#include <algorithm>
#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "engine.h"
#include "scene.h"
#include "obj_loader.h"
#include "light.h"

int main(){
	Engine *ariamis = Engine::getInstance();
	ariamis->createWindow(1024, 1024, "Example");

	Scene s;

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
	Camera &camera = s.cameras[0];
	ariamis->registerKeyEvent(GLFW_KEY_W, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, 0, 1), dt * speedMultiplier);
	});
	ariamis->registerKeyEvent(GLFW_KEY_S, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, 0, -1), dt * speedMultiplier);
	});
	ariamis->registerKeyEvent(GLFW_KEY_A, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(-1, 0, 0), dt * speedMultiplier);
	});
	ariamis->registerKeyEvent(GLFW_KEY_D, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(1, 0, 0), dt * speedMultiplier);
	});
	ariamis->registerKeyEvent(GLFW_KEY_SPACE, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, 1, 0), dt * speedMultiplier);
	});
	ariamis->registerKeyEvent(GLFW_KEY_LEFT_SHIFT, [&camera, speedMultiplier](float dt){
		camera.moveRelative(glm::vec3(0, -1, 0), dt * speedMultiplier);
	});

	// Need to set yaw and pitch based on the original camera position
	float yaw = 90.0f;
	float pitch = 0.0f;
	double lastX, lastY;
	float sensitivity = 0.1f;
	glfwGetCursorPos(ariamis->getWindow(), &lastX, &lastY);
	ariamis->registerMouseMoveEvent([&yaw, &pitch, &lastX, &lastY, &camera, sensitivity](double x, double y){
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

	ariamis->playScene(s);
}
