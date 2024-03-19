#version 140
in vec3 vertexPos3D;
in vec2 vUV;
out vec2 uv;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 transformMat;

out float fog_amount;
uniform float fog_density;

uniform vec4 instanceOffset[20];

void main()
{
	vec4 offset = instanceOffset[gl_InstanceID];
	vec4 v = vec4(vertexPos3D.xyz, 1);
	vec4 posInWorld = transformMat * v;
	v = projMat * viewMat * posInWorld;

	if (any(greaterThan(offset, vec4(0.0)))) {
        gl_Position = vec4(v.xyz, 1.0); //instanced specific offset application 
    } else {
        gl_Position = v;
    }
	uv = vUV;

	//fog calculation
	vec4 pos_rel_eye = viewMat * posInWorld;
	float distance = length(pos_rel_eye.xyz);
	fog_amount = 1.0 - exp(-pow(distance * fog_density, 2.0));
	fog_amount = clamp(fog_amount, 0.0, 1.0);
}