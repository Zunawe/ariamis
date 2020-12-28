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
