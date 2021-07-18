#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;


//  out vec3 ourColor;
out vec2 TexCoord;

// PVM
uniform mat4 modelVs;
uniform mat4 viewVs;
uniform mat4 projVs;

void main()
{
 
	// note that we read the multiplication from right to left
    gl_Position = projVs * viewVs * modelVs * vec4(aPos, 1.0);

	// ourColor = aColor;

    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
