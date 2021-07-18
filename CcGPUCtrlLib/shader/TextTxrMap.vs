#version 400 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

// PVM
uniform mat4 modelVs;
uniform mat4 viewVs;
uniform mat4 projVs;

mat4 projection;

void main()
{
    projection  = projVs * viewVs * modelVs;
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);  // x & y
    TexCoords = vertex.zw;  // z & w
}  