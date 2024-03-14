#version 140
in vec3 vertexPos3D;
in vec2 vUV;
out vec2 uv;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 transformMat;

void main()
{
	gl_Position = projMat * viewMat * transformMat * vec4(vertexPos3D,1);
	uv = vUV;
}