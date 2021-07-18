#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 modelVs;
uniform mat4 viewVs;
uniform mat4 projVs;


void main()
{
    gl_Position = projVs * viewVs * modelVs * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  