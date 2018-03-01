#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuseMap;
	vec3 specular;
	sampler2D specularMap;
	float shininess;
};

struct DirectionalLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float kc;
	float kl;
	float kq;
};

in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexTextureCoordinate;
in vec3 fragPos;

in mat3 TBN;

uniform Material material;
uniform DirectionalLight dLights[4];
uniform PointLight pLights[4];
uniform vec3 cameraPos;

out vec4 fragColor;

vec3 getLightColor(vec3 a, vec3 d, vec3 s, vec3 normal, vec3 toCamera, vec3 toLight){
	normal = normalize(normal);
	toCamera = normalize(toCamera);
	toLight = normalize(toLight);
	vec3 ambient, diffuse, specular;

	ambient = vec3(texture(material.diffuseMap, vertexTextureCoordinate)) * a * material.ambient;

	float multiplier = max(dot(normal, toLight), 0);
	diffuse = vec3(texture(material.diffuseMap, vertexTextureCoordinate)) * (d * material.diffuse) * multiplier;

	vec3 reflected = reflect(-toLight, normal);
	float shinyMultiplier = pow(max(dot(toCamera, reflected), 0), material.shininess);
	specular = vec3(texture(material.specularMap, vertexTextureCoordinate)) * (s * material.specular) * shinyMultiplier;

	return ambient + diffuse + specular;
}

vec3 lighting(){
	vec3 toCamera = cameraPos - fragPos;
	vec3 color;

	for(int i = 0; i < dLights.length(); ++i){
		color += getLightColor(dLights[i].ambient, dLights[i].diffuse, dLights[i].specular, vertexNormal, toCamera, -dLights[i].direction);
	}
	for(int i = 0; i < pLights.length(); ++i){
		float distanceToLight = length(pLights[i].position - fragPos);
		float attenuation = pLights[i].kc + (pLights[i].kl * distanceToLight) + (pLights[i].kq * distanceToLight * distanceToLight);
		attenuation = attenuation > 0.0 ? 1 / attenuation : 1.0;

		color += attenuation * getLightColor(pLights[i].ambient, pLights[i].diffuse, pLights[i].specular, vertexNormal, toCamera, pLights[i].position - fragPos);
	}
	
	return color;
}

void main(){
	fragColor = vec4(lighting(), 1.0);
}
