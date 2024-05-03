#version 140

// Inputs to vertex shader
in vec3 vertexPos3D;
in vec2 vUV;
// Output
out vec2 uv;
out vec4 ParticleColour;

// MVP transformations
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 colour;

// Shader main code
void main() {
	// Convert input vertex to vec4 
	vec4 v = vec4(vertexPos3D.xyz, 1);
	ParticleColour = colour;
	// Transform
	v = projection * view * transform * v;
	gl_Position = v;
	// As well as uv.  Don't transform uv
	uv = vUV;
}