#version 330

// Input to fragment shader
in vec2 uv;
in vec4 ParticleColour;
// Texture
uniform sampler2D sampler;

// Final fragment colour
out vec4 fragmentColour;

// Shader body
void main()
{
	// Determine fragment colour from texture based on uv co-oords
	vec4 texColour = texture(sampler, uv);

	vec4 finalColour = texColour * ParticleColour.a;

	fragmentColour = vec4(finalColour.rgb, finalColour.a * ParticleColour.a);
}

