#version 450 core

in vec3 vertexColor;

out vec4 fragColor;

void main(){
	fragColor = vec4(vertexColor.rgb, 1.0);
}
