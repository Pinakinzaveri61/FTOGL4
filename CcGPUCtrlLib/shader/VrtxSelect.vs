#version 400 core

layout (location = 0) in vec3 aPos;
  

// PVM
uniform mat4 modelVs;
uniform mat4 viewVs;
uniform mat4 projVs;


void main()
{

	// note that we read the multiplication from right to left
    gl_Position = projVs * viewVs * modelVs * vec4(aPos, 1.0);

}
