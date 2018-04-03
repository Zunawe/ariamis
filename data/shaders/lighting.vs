#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;

out vec2 vertexTextureCoordinates;

void main(){
	vertexTextureCoordinates = aTextureCoordinates;
	gl_Position = vec4(aPosition, 1.0);
}
