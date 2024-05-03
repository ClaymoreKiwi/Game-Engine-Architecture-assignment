#version 330

// Input to fragment shader
in vec2 uv;
in vec3 outNorm;
in vec3 posW;
in float fog_amount;

// Texture
uniform vec3 fog_colour;
uniform sampler2D sampler;
uniform vec3 viewPos;
// Final fragment colour
out vec4 fragmentColour;

uniform vec3 lightColour;
uniform vec3 ambient;
uniform vec3 lightPos;
const float shininess = 32.0;
const float specularStrength = 0.3;
// Shader body
void main()
{
	// Determine fragment colour from texture based on uv co-oords
	vec4 texColour = texture(sampler, uv).rgba;

	vec3 normaliseNormal = normalize(outNorm);

	vec3 lightDir = normalize(lightPos - posW);

	float diffIllumination = max(dot(normaliseNormal, lightDir), 0.0);
	
	vec3 diffuse = diffIllumination * lightColour;
	vec3 viewDir = normalize(viewPos - posW);
	vec3 reflectionDir = reflect(-lightDir, normaliseNormal);

	float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shininess);

	vec3 specular = specularStrength * spec * lightColour;

	vec3 finalColour = (ambient + diffuse) * texColour.rgb + specular;

	vec4 ligthing = vec4(finalColour, texColour.a);
	fragmentColour = mix(ligthing, vec4(fog_colour, 1), fog_amount);
}