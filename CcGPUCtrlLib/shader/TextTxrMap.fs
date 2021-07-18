#version 400 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    vec3 textColor2 = vec3(1.0, 0.0, 0.0);

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
   // color = vec4(textColor2, 1.0) * sampled;
} 