#version 330

in vec3 vertexPos3D;
in vec3 vertexNormal;
in vec2 vUV;

out float fog_amount;
out vec2 uv;
out vec3 posW;
out vec3 outNorm;
out vec4 fragPosLight;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 transformMat;
uniform mat4 lightProjection;

uniform float fog_density;

void main()
{
    vec4 v = vec4(vertexPos3D.xyz, 1);
    posW = vec3(viewMat * transformMat * v);
    outNorm = vec3(viewMat * transformMat * vec4(vertexNormal, 0.0));
    vec4 posInWorld = transformMat * v;
    
    v = projMat * viewMat * posInWorld;
    fragPosLight = lightProjection * posInWorld;
    gl_Position = v;
    uv = vUV;

    // Fog calculation
    vec4 pos_rel_eye = viewMat * posInWorld;
    float distance = length(pos_rel_eye.xyz);
    fog_amount = 1.0 - exp(-pow(distance * fog_density, 2.0));
    fog_amount = clamp(fog_amount, 0.0, 1.0);
}