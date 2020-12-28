#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;

out vec2 vTextureCoordinates;

void main(){
	vTextureCoordinates = aTextureCoordinates;
	gl_Position = vec4(aPosition, 1.0);
}
