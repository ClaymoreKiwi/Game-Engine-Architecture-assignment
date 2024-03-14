#version 140

in vec3 vertexPos3D;
out vec3 texCoord;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 v = vec4(vertexPos3D.xyz, 1);
    v = projection * view * v;
    gl_Position = v;
    texCoord = vertexPos3D;
}


