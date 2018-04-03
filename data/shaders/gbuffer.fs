#version 330 core
layout(location = 0) out vec3 gPosition;

in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexTextureCoordinate;
in vec3 fragPos;

uniform vec3 cameraPos;

void main(){
	gPosition = fragPos;
}

