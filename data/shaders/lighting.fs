#version 330 core

struct Light{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// Point
	float kc;
	float kl;
	float kq;

	// Spot
	vec3 direction;
	float cosAngle;
};

in vec2 vTextureCoordinates;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpecular;

uniform mat4 view;

uniform Light lights[4];

out vec4 fColor;

void main(){
	vec3 position = texture(gPosition, vTextureCoordinates).xyz;
	vec3 normal = texture(gNormal, vTextureCoordinates).xyz;
	vec4 albedoSpecular = texture(gAlbedoSpecular, vTextureCoordinates);
	vec3 albedo = albedoSpecular.rgb;
	vec3 spec = albedoSpecular.aaa;

	vec3 toCamera = normalize(-position);

	vec3 ambient, diffuse, specular;
	for(int i = 0; i < 4; ++i){
		Light light = lights[i];
		light.position = view * light.position;
		light.direction = (view * vec4(light.direction, 0.0)).xyz;

		// Directional Light
		vec3 toLight = normalize(light.position.w != 0.0 ? light.position.xyz - position : -light.position.xyz);

		// Point Light
		float lightDistance = length(light.position.xyz - position);
		float denom = light.kc == 0.0 ? 1.0 : (light.kc + (light.kl * lightDistance) + (light.kq * lightDistance * lightDistance));
		float attenuation = 1.0 / denom;

		// Spotlight
		float spotlightMultiplier = light.cosAngle == 0.0 ? 1.0 : step(light.cosAngle, dot(normalize(light.direction), -toLight));

		// Ambient
		ambient += attenuation * albedo * light.ambient;

		// Diffuse
		float multiplier = max(dot(normal, toLight), 0);
		diffuse += spotlightMultiplier * attenuation * albedo * light.diffuse * multiplier;

		// Specular
		vec3 reflected = reflect(-toLight, normal);
		float shinyMultiplier = pow(max(dot(toCamera, reflected), 0), 32.0);
		specular += spotlightMultiplier * attenuation * spec * light.specular * shinyMultiplier;
	}

	fColor = vec4(ambient + diffuse + specular, 1.0);
}
