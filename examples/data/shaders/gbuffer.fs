#version 330 core

struct Material{
	sampler2D diffuseMap;
	sampler2D specularMap;
};

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gColorSpec;

in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexTextureCoordinates;
in vec3 fragPos;

uniform Material material;
uniform vec3 cameraPos;

void main(){
	gPosition = fragPos;
	gNormal = normalize(vertexNormal);
	gColorSpec.rgb = texture(material.diffuseMap, vertexTextureCoordinates).rgb;
	gColorSpec.a = texture(material.specularMap, vertexTextureCoordinates).r;
}
