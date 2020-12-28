#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTextureCoordinates;

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 modelViewProjection;

out vec3 vNormal;
out vec3 vColor;
out vec2 vTextureCoordinates;
out vec3 fPos;

void main(){
	gl_Position = modelViewProjection * vec4(aPos, 1.0);

	vNormal = normalModel * aNormal;
	vColor = aColor;
	vTextureCoordinates = aTextureCoordinates;

	fPos = vec3(view * model * vec4(aPos, 1.0));
}
