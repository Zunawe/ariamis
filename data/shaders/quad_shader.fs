#version 330 core

in vec2 vTextureCoordinates;

uniform sampler2D image;

out vec4 fColor;

void main(){
	fColor = texture(image, vTextureCoordinates);
}
