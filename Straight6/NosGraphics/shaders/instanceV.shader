#version 330

// Inputs to vertex shader
in vec3 vertexPos3D;
in vec3 vertexNormal;
in vec2 vUV;

in mat4 instanceTransform;

// Output
out vec2 uv;
out vec3 posW;
out vec3 outNorm;
out float fog_amount;

// View project matrices.  Note no model matrix as each instance
// has its own matrix
uniform mat4 view;
uniform mat4 projection;
uniform float fog_density;

// Shader main code
void main() {
	// Convert input vertex to 4D 
	vec4 v = vec4(vertexPos3D.xyz, 1);
	posW = vec3(view * instanceTransform * v);
    outNorm = vec3(view * instanceTransform * vec4(vertexNormal, 0.0));
	vec4 posInWorld = instanceTransform * v;
	v = projection * view * posInWorld;

	// Pass transformed vertex to next stage
	gl_Position = v;
	uv = vUV;

	// Fog calculation
    vec4 pos_rel_eye = view * posInWorld ;
    float distance = length(pos_rel_eye.xyz);
    fog_amount = 1.0 - exp(-pow(distance * fog_density, 2.0));
    fog_amount = clamp(fog_amount, 0.0, 1.0);
}