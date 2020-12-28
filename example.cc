#include <iostream>

#include "glm/ext.hpp"

#include "engine.h"
#include "scene.h"
#include "obj_loader.h"
#include "light.h"

int main(){
	Engine::createWindow("Example");

	Scene s;

	Texture *rockDiffuseMap = new Texture("data/textures/rock_diffuse.png");
	Texture *rockSpecularMap = new Texture("data/textures/rock_specular.png");

	// Objects
	std::shared_ptr<Object> rock(new Object());
	rock->renderer.setMesh(loadMeshFromObj("data/objects/rock.obj"));
	rock->renderer.getMaterial().diffuseMap = rockDiffuseMap;
	rock->renderer.getMaterial().specularMap = rockSpecularMap;

	rock->position = glm::vec3(3, 3, 3);

	s.objects.push_back(rock);

	for(int i = 0; i < 6; ++i){
		std::shared_ptr<Object> plane(new Object());
		plane->renderer.setMesh(loadMeshFromObj("data/objects/cube.obj"));
		s.objects.push_back(plane);

		plane->position[i / 2] = ((2 * (i % 2)) - 1) * 10.0f;
		plane->scale(9.0f);
	}

	// Lights
	std::shared_ptr<PointLight> pointLight(new PointLight());
	pointLight->position = glm::vec4(1, 0.1f, 2, 1);
	pointLight->diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
	pointLight->ambient = glm::vec3(0.7f, 0.7f, 0.7f);
	pointLight->specular = glm::vec3(0.4f, 0.4f, 0.4f);
	pointLight->kc = 1.0f;
	pointLight->kl = 0.05f;
	pointLight->kq = 0.001f;
	s.lights.push_back(pointLight);

	std::shared_ptr<SpotLight> spotLight(new SpotLight());
	spotLight->position = glm::vec4(0, 0, 0, 1);
	spotLight->diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	spotLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight->specular = glm::vec3(0.8f, 0.8f, 0.8f);
	spotLight->direction = glm::vec3(1.0f, 0.5f, 0.2f);
	spotLight->cosAngle = cos(glm::radians(30.0f));
	s.lights.push_back(spotLight);

	// Input
	float speedMultiplier = 2.0f;
	Camera &camera = s.camera;
	Engine::registerKeyEvent(GLFW_KEY_ESCAPE, [](float /*dt*/){
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
	float yaw = glm::degrees(atan(camera.getForward().z / camera.getForward().x));
	float pitch = glm::degrees(asin(camera.getForward().y));
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

	delete rockDiffuseMap;
	delete rockSpecularMap;
}
