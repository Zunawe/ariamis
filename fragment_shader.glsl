#version 450 core

in vec3 vectorColor;

out vec4 fragColor;

void main(){
	fragColor = vec4(vectorColor.rgb, 1.0f);
}
