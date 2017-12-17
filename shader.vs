#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTextureCoordinate;

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 coordinateTransform;

out vec3 vertexNormal;
out vec3 vertexColor;
out vec2 vertexTextureCoordinate;

out vec3 fragPos;

void main(){
	gl_Position = coordinateTransform * vec4(aPos, 1.0);

	vertexNormal = normalModel * aNormal;
	vertexColor = aColor;
	vertexTextureCoordinate = aTextureCoordinate;

	fragPos = vec3(model * vec4(aPos, 1.0));
}
