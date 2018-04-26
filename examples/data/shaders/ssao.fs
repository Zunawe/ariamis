#version 330 core

in vec2 vertexTextureCoordinates;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D rotationNoise;

uniform vec3 samples[32];
uniform mat4 projection;
uniform mat4 view;

out float fragmentColor;

const vec2 noiseScale = vec2(1920.0 / 4.0 / 2.0, 1080.0 / 4.0 / 2.0);
const float radius = 5.0;
const float bias = 0.1;

void main(){
	vec3 position = texture(gPosition, vertexTextureCoordinates).xyz;
	vec3 normal = texture(gNormal, vertexTextureCoordinates).xyz;
	vec3 kernelRotation = normalize(texture(rotationNoise, vertexTextureCoordinates * noiseScale)).xyz;

	vec3 tangent = normalize(kernelRotation - (normal * dot(kernelRotation, normal)));
	vec3 bitangent = cross(normal, tangent);

	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;

	for(int i = 0; i < 32; ++i){
		vec3 sample = TBN * samples[i];
		sample = position + (sample * radius);

		vec4 offset = projection * vec4(sample, 1.0);
		offset /= offset.w;
		offset = (offset + 1.0) / 2.0;

		float sampleDepth = texture(gPosition, offset.xy).z;

		float rangeScale = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));

		occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeScale;
	}

	fragmentColor = 1.0 - (occlusion / 32.0);
}
