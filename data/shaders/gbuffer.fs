#version 330 core

struct Material{
	sampler2D diffuseMap;
	sampler2D specularMap;
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
	gAlbedoSpecular.rgb = texture(material.diffuseMap, vTextureCoordinates).rgb;
	gAlbedoSpecular.a = texture(material.specularMap, vTextureCoordinates).r;
}
