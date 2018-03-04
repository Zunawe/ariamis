#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	sampler2D diffuseMap;
	vec3 specular;
	sampler2D specularMap;
	float shininess;
};

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

in vec3 vertexNormal;
in vec3 vertexColor;
in vec2 vertexTextureCoordinate;
in vec3 fragPos;

in mat3 TBN;

uniform Material material;
uniform Light lights[4];
uniform vec3 cameraPos;

out vec4 fragColor;

vec3 lighting(){
	vec3 normal = normalize(vertexNormal);
	vec3 toCamera = normalize(cameraPos - fragPos);

	vec3 ambient, diffuse, specular;
	for(int i = 0; i < 4; ++i){
		Light light = lights[i];

		// Directional Light
		vec3 toLight = normalize(light.position.w != 0.0 ? light.position.xyz - fragPos : -light.position.xyz);

		// Point Light
		float lightDistance = length(light.position.xyz - fragPos);
		float denom = light.kc == 0.0 ? 1.0 : (light.kc + (light.kl * lightDistance) + (light.kq * lightDistance * lightDistance));
		float attenuation = 1.0 / denom;

		// Spotlight
		float spotlightMultiplier = light.angle == 0.0 ? 1.0 : step(light.cosAngle, dot(normalize(light.direction), -toLight));

		// Ambient
		ambient += attenuation * vec3(texture(material.diffuseMap, vertexTextureCoordinate)) * light.ambient * material.ambient;

		// Diffuse
		float multiplier = max(dot(normal, toLight), 0);
		diffuse += spotlightMultiplier * attenuation * vec3(texture(material.diffuseMap, vertexTextureCoordinate)) * (light.diffuse * material.diffuse) * multiplier;

		// Specular
		vec3 reflected = reflect(-toLight, normal);
		float shinyMultiplier = pow(max(dot(toCamera, reflected), 0), material.shininess);
		specular += spotlightMultiplier * attenuation * vec3(texture(material.specularMap, vertexTextureCoordinate)) * (light.specular * material.specular) * shinyMultiplier;
	}
	return ambient + diffuse + specular;
}

void main(){
	fragColor = vec4(lighting(), 1.0);
}
