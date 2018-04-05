#version 330 core

in vec2 vertexTextureCoordinates;

uniform sampler2D source;

out float fragmentColor;

const int r = 3;

void main(){
	float result;
	for(int i = -r; i <= r; ++i){
		result += texture(source, vertexTextureCoordinates - vec2(0, i / 1920.0)).r;
	}
	fragmentColor = result / ((2 * r) + 1);
}
