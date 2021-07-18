#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inNormal;
// layout (location = 2) in vec2 aTexCoord;

 out vec3 FragPos;
 out vec3 Normal;

uniform mat4 modelVs;
uniform mat4 viewVs;
uniform mat4 projVs;

void main()
{
 //  FragPos = vec3(modelVs * vec4(aPos, 1.0));
   
   FragPos = vec3(projVs * modelVs * vec4(aPos, 1.0));

   Normal = mat3(transpose(inverse(modelVs))) * inNormal;  

   // gl_Position = projVs * viewVs * vec4(FragPos, 1.0);
   gl_Position = projVs * viewVs * modelVs * vec4(aPos, 1.0);
}