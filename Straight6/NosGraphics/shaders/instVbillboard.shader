#version 330

// Inputs to vertex shader
in vec3 vertexPos3D;
in vec2 vUV;

in mat4 inst_Pos;
in float inst_Fade;

// Output
out vec2 uv;
out vec4 ParticleColour;

// MVP transformations
uniform mat4 view;
uniform mat4 projection;


// Shader main code
void main() {
    vec4 v = vec4(vertexPos3D.xyz, 1.0);
    vec4 posInWorld = inst_Pos * v;
    // Transform
    v = projection * view * posInWorld;
    gl_Position = v;

    // Pass UV without transformation
    uv = vUV;

    // Set particle color
    ParticleColour = vec4(1.0, 1.0, 1.0, inst_Fade);
}