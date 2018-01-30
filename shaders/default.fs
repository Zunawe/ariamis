#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuseMap;
	vec3 specular;
	sampler2D specularMap;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexTextureCoordinate;
in vec3 fragPos;

uniform Material material;
uniform Light light[4];
uniform vec3 cameraPos;

out vec4 fragColor;

vec3 lighting(){
	vec3 normal = normalize(vertexNormal);
	vec3 toCamera = normalize(cameraPos - fragPos);

	vec3 ambient, diffuse, specular;
	for(int i = 0; i < 4; ++i){
		ambient += vec3(texture(material.diffuseMap, vertexTextureCoordinate)) * light[i].ambient * material.ambient;

		vec3 toLight = normalize(light[i].position - fragPos);
		float multiplier = max(dot(normal, toLight), 0);
		diffuse += vec3(texture(material.diffuseMap, vertexTextureCoordinate)) * (light[i].diffuse * material.diffuse) * multiplier;

		vec3 reflected = reflect(-toLight, normal);
		float shinyMultiplier = pow(max(dot(toCamera, reflected), 0), material.shininess);
		specular += vec3(texture(material.specularMap, vertexTextureCoordinate)) * (light[i].specular * material.specular) * shinyMultiplier;
	}
	return ambient + diffuse + specular;
}

void main(){
	fragColor = vec4(lighting(), 1.0);
}
