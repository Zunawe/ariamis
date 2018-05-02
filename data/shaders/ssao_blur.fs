#version 330 core

in vec2 vertexTextureCoordinates;

uniform sampler2D source;
uniform int axis;

out float fragmentColor;

const int r = 2;

void main(){
	float result;
	float totalWeight;
	for(int i = -r; i <= r; ++i){
		result += abs(r - (i / 2.0)) * texture(source, vertexTextureCoordinates - vec2((i / 1920.0 * 2.0) * (1 - axis), (i / 1080.0 * 2.0) * axis)).r;
		totalWeight += abs(r - (i / 2.0));
	}
	fragmentColor = result / totalWeight;
}
