#include <iostream>

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "engine.h"
#include "scene.h"
#include "obj_loader.h"
#include "light.h"

int main(){
	Engine ariamis = Engine::getInstance();
	ariamis.createWindow(1024, 1024, "Graphic");

	Scene s(ariamis.getWidth(), ariamis.getHeight());

	Object sphere;
	sphere.renderer.setMesh(loadMeshFromObj("data/objects/sphere.obj"));
	s.objects.push_back(std::shared_ptr<Object>(&sphere));

	std::shared_ptr<PointLight> pointLight(new PointLight());
	pointLight->position = glm::vec4(1, 0, -2, 1);
	pointLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight->kc = 1.0f;
	pointLight->kl = 0.05f;
	pointLight->kq = 0.001f;
	s.lights.push_back(pointLight);

	ariamis.playScene(s);
}
