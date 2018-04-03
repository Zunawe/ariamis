#version 330 core

in vec2 vertexTextureCoordinates;

uniform sampler2D image;
uniform int test;

out vec4 fragmentColor;

void main(){
	// fragmentColor = vec4(0, test, 0, 1);
	fragmentColor = texture(image, vertexTextureCoordinates);
}
