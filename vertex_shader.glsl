#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTextureCoordinate;

uniform mat4 model;

out vec3 vertexColor;
out vec2 vertexTextureCoordinate;

void main(){
	gl_Position = model * vec4(aPos.xyz, 1.0);
	gl_PointSize = 3.0;

	vertexColor = aColor;
	vertexTextureCoordinate = aTextureCoordinate;
}
