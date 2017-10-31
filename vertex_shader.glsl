#version 450 core

layout(location = 0) in vec2 aPos;

void main(){
	gl_Position = vec4(aPos.xy, 0.0f, 1.0f);
	gl_PointSize = 3.0f;
}
