#include "engine.h"

#include <stdexcept>

#include "shader.h"
#include "material.h"
#include "scene.h"

unsigned int Engine::gBuffer = 0;
unsigned int Engine::gPosition = 0;
unsigned int Engine::gNormal = 0;
unsigned int Engine::gAlbedoSpecular = 0;
Shader Engine::lightingShader;
Shader Engine::gBufferShader;

unsigned int Engine::quadVAO = 0;

std::map<int, std::vector<std::function<void(float)>>> Engine::keyCallbacks;
std::vector<std::function<void(double, double)>> Engine::mouseMoveCallbacks;
float Engine::lastTime = 0.0f;
float Engine::deltaTime = 0.0f;
unsigned int Engine::width = 0;
unsigned int Engine::height = 0;
GLFWwindow *Engine::window;

const char *gBufferVertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTextureCoordinates;

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 modelViewProjection;

out vec3 vNormal;
out vec3 vColor;
out vec2 vTextureCoordinates;
out vec3 fPos;

void main(){
	gl_Position = modelViewProjection * vec4(aPos, 1.0);

	vNormal = normalModel * aNormal;
	vColor = aColor;
	vTextureCoordinates = aTextureCoordinates;

	fPos = vec3(view * model * vec4(aPos, 1.0));
}
)";

const char *gBufferFragmentShaderSource = R"(
#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuseMap;
	vec3 specular;
	sampler2D specularMap;
	float shininess;
};

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpecular;

in vec3 vNormal;
in vec3 vColor;
in vec2 vTextureCoordinates;
in vec3 fPos;

uniform Material material;
uniform vec3 cameraPos;

void main(){
	gPosition = fPos;
	gNormal = normalize(vNormal);
	gAlbedoSpecular.rgb = material.diffuse * texture(material.diffuseMap, vTextureCoordinates).rgb;
	gAlbedoSpecular.a = material.specular.r * texture(material.specularMap, vTextureCoordinates).r;
}
)";

const char *lightingVertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;

out vec2 vTextureCoordinates;

void main(){
	vTextureCoordinates = aTextureCoordinates;
	gl_Position = vec4(aPosition, 1.0);
}
)";

const char *lightingFragmentShaderSource = R"(
#version 330 core

struct Light{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Point
	float kc;
	float kl;
	float kq;

	// Spot
	vec3 direction;
	float cosAngle;
};

in vec2 vTextureCoordinates;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpecular;

uniform mat4 view;

uniform Light lights[4];

out vec4 fColor;

void main(){
	vec3 position = texture(gPosition, vTextureCoordinates).xyz;
	vec3 normal = texture(gNormal, vTextureCoordinates).xyz;
	vec4 albedoSpecular = texture(gAlbedoSpecular, vTextureCoordinates);
	vec3 albedo = albedoSpecular.rgb;
	vec3 spec = albedoSpecular.aaa;

	vec3 toCamera = normalize(-position);

	vec3 ambient, diffuse, specular;
	for(int i = 0; i < 4; ++i){
		Light light = lights[i];
		light.position = view * light.position;
		light.direction = (view * vec4(light.direction, 0.0)).xyz;

		// Directional Light
		vec3 toLight = normalize(light.position.w != 0.0 ? light.position.xyz - position : -light.position.xyz);

		// Point Light
		float lightDistance = length(light.position.xyz - position);
		float denom = light.kc == 0.0 ? 1.0 : (light.kc + (light.kl * lightDistance) + (light.kq * lightDistance * lightDistance));
		float attenuation = 1.0 / denom;

		// Spotlight
		float spotlightMultiplier = light.cosAngle == 0.0 ? 1.0 : step(light.cosAngle, dot(normalize(light.direction), -toLight));

		// Ambient
		ambient += attenuation * albedo * light.ambient;

		// Diffuse
		float multiplier = max(dot(normal, toLight), 0);
		diffuse += spotlightMultiplier * attenuation * albedo * light.diffuse * multiplier;

		// Specular
		vec3 reflected = reflect(-toLight, normal);
		float shinyMultiplier = pow(max(dot(toCamera, reflected), 0), 32.0);
		specular += spotlightMultiplier * attenuation * spec * light.specular * shinyMultiplier;
	}

	fColor = vec4(ambient + diffuse + specular, 1.0);
}
)";

/**
 * Initialization that should happen after a GL context has been created.
 */
void Engine::postContextCreation(){
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	glfwSetCursorPosCallback(window, mouseMoveCallback);

	glEnable(GL_DEPTH_TEST);

	lightingShader.loadSource(lightingVertexShaderSource, GL_VERTEX_SHADER);
	lightingShader.loadSource(lightingFragmentShaderSource, GL_FRAGMENT_SHADER);
	lightingShader.link();

	gBufferShader.loadSource(gBufferVertexShaderSource, GL_VERTEX_SHADER);
	gBufferShader.loadSource(gBufferFragmentShaderSource, GL_FRAGMENT_SHADER);
	gBufferShader.link();

	initializeGBuffer();

	unsigned char whitePixel[3] = {255, 255, 255};
	Texture::DEFAULT_TEXTURE = Texture();
	Texture::DEFAULT_TEXTURE.loadRaw(1, 1, whitePixel);

	Material::DEFAULT_MATERIAL = Material();

	glfwSwapInterval(0);
}

void Engine::initializeGBuffer(){
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	glGenTextures(1, &gAlbedoSpecular);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecular, 0);

	unsigned int attachments[]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, attachments);

	unsigned int depthRBO;
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * Reset Engine for use again during this runtime if necessary.
 */
void Engine::cleanUp(){
	glfwDestroyWindow(window);
	keyCallbacks.clear();
	mouseMoveCallbacks.clear();
}

/**
 * Create the GLFW window that will contain the GL context and initialize
 * things that require said context. You should do this before creating any
 * other instances of anything from this library.
 */
GLFWwindow* Engine::createWindow(const char *name, unsigned int width, unsigned int height, bool fullscreen){
	glfwDestroyWindow(window);

	if(!glfwInit()){
		throw std::runtime_error("Failed to initialize GLFW");
	}

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	if(fullscreen){
		width = mode->width;
		height = mode->height;
		window = glfwCreateWindow(width, height, name, glfwGetPrimaryMonitor(), NULL);
	}
	else{
		window = glfwCreateWindow(width, height, name, NULL, NULL);
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if(!gladLoadGL()){
		throw std::runtime_error("Failed to Load GLAD");
	}

	postContextCreation();

	return window;
}

/**
 * Run a scene until the window is closed. This will start the main loop,
 * poll input events, and draw frames.
 * 
 * @param scene the scene to display and run.
 */
void Engine::playScene(Scene &scene){
	for(auto it = scene.objects.begin(); it != scene.objects.end(); ++it){
		(*it)->renderer.setShader(gBufferShader);
	}

	while(!glfwWindowShouldClose(window)){
		float newTime = glfwGetTime();
		deltaTime = newTime - lastTime;
		lastTime = newTime;

		glfwPollEvents();
		processInputs();

		scene.update();

		// G-Buffer Pass
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			scene.draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
		lightingShader.setUniform("gPosition", 0);
		lightingShader.setUniform("gNormal", 1);
		lightingShader.setUniform("gAlbedoSpecular", 2);

		// Lighting Pass
		glViewport(0, 0, width, height);
		lightingShader.use();
		lightingShader.setUniform("view", scene.camera.getViewMatrix());

		for(unsigned int i = 0; i < scene.lights.size(); ++i){
			scene.lights[i]->setUniforms(lightingShader, "lights[" + std::to_string(i) + "]");
		}

		drawQuad();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
}

void Engine::drawQuad(){
	if(!quadVAO){
		unsigned int quadVBO;
		float attributes[]{
			 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);

		glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(attributes), &attributes, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

/**
 * Loops through all the callbacks for keypresses and calls the ones that have
 * a key pressed.
 */
void Engine::processInputs(){
	for(auto it = keyCallbacks.begin(); it != keyCallbacks.end(); ++it){
		if(glfwGetKey(window, it->first) == GLFW_PRESS){
			for(auto fp = it->second.begin(); fp != it->second.end(); ++fp){
				(*fp)(deltaTime);
			}
		}
	}
}

/**
 * Closes the window after the current frame finishes rendering.
 */
void Engine::quit(){
	glfwSetWindowShouldClose(window, true);
}

/**
 * Registers a new key callback. This function will be called every frame key
 * is being pressed.
 * 
 * @param key a GLFW key code (e.g. GLFW_KEY_W)
 * @param func the function to run while the key is pressed. It will be passed
 * the time since the last frame.
 */
void Engine::registerKeyEvent(int key, std::function<void(float)> func){
	keyCallbacks[key].push_back(func);
}

/**
 * Registers a new mouse movement callback. This function will be called every
 * time the mouse position changes.
 * 
 * @param func the function to run when the mouse moves. It will be passed
 * the screen space x and y coordinates.
 */
void Engine::registerMouseMoveEvent(std::function<void(double, double)> func){
	mouseMoveCallbacks.push_back(func);
}

/**
 * Gets the GLFW window pointer.
 * 
 * @return the GLFW window pointer.
 */
GLFWwindow* Engine::getWindow(){
	return window;
}

int Engine::getWidth(){
	return width;
}

int Engine::getHeight(){
	return height;
}

float Engine::getDeltaTime(){
	return deltaTime;
}

float Engine::getTime(){
	return lastTime;
}

/**
 * GLFW callback for when the window is resized. Resets the viewport and records
 * the new width and height.
 */
void Engine::resizeWindow(GLFWwindow * /*window*/, int newWidth, int newHeight){
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth;
	height = newHeight;
	glDeleteFramebuffers(1, &gBuffer);
	initializeGBuffer();
}

/**
 * GLFW callback for when the mouse moves. Calls all the registered callbacks,
 * so this function can dynamically add or remove functionality.
 */
void Engine::mouseMoveCallback(GLFWwindow * /*window*/, double x, double y){
	for(auto it = mouseMoveCallbacks.begin(); it != mouseMoveCallbacks.end(); ++it){
		(*it)(x, y);
	}
}
