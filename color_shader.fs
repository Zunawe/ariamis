#version 450 core

in vec3 vertexColor;

uniform sampler2D texture1;

out vec4 fragColor;

void main(){
	fragColor = vec4(vertexColor, 1.0);
}
