#version 330

in vec3 Pos;

uniform mat4 lightProjection;
uniform mat4 model;

void main()
{
	gl_Position = lightProjection * model * vec4(Pos, 1.0);
}