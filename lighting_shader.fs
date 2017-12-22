#version 450 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 vertexNormal;
in vec2 vertexTextureCoordinate;
in vec3 fragPos;

uniform sampler2D texture0;
uniform Material material;
uniform Light light;
uniform vec3 cameraPos;

out vec4 fragColor;

void main(){
	vec3 ambient = light.ambient * material.ambient;

	vec3 normal = normalize(vertexNormal);
	vec3 toLight = normalize(light.pos - fragPos);
	float multiplier = max(dot(normal, toLight), 0);
	vec3 diffuse = (light.diffuse * material.diffuse) * multiplier;

	vec3 toCamera = normalize(cameraPos - fragPos);
	vec3 reflected = reflect(-toLight, normal);
	float shinyMultiplier = pow(max(dot(toCamera, reflected), 0), material.shininess);
	vec3 specular = (light.specular * material.specular) * shinyMultiplier;

	fragColor = texture(texture0, vertexTextureCoordinate) * vec4(ambient + diffuse + specular, 1.0);
}
