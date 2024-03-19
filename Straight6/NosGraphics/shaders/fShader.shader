#version 140
in vec2 uv;
in float fog_amount;

uniform vec3 fog_colour;

uniform sampler2D sampler;

out vec4 fragmentColour;

void main()
{
	vec4 colour = texture(sampler, uv);
	fragmentColour = mix(colour, vec4(fog_colour, 1), fog_amount);
}