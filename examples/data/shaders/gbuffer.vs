#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aColor;
layout(location = 4) in vec2 aTextureCoordinates;

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 modelViewProjection;

out vec3 vertexNormal;
out vec3 vertexColor;
out vec2 vertexTextureCoordinates;
out vec3 fragPos;

void main(){
	gl_Position = modelViewProjection * vec4(aPos, 1.0);

	vertexNormal = normalModel * aNormal;
	vec3 vertexTangent = normalModel * aTangent;
	vertexColor = aColor;
	vertexTextureCoordinates = aTextureCoordinates;

	fragPos = vec3(view * model * vec4(aPos, 1.0));
}
