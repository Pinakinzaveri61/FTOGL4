#version 400 core

out vec4 FragColor;

// Color per Shape, i.e. 'ourColor' set before drawing the shape and is applied to all the vertices of the shape.
// uniform vec4 ourColor;

 uniform int RedU;
 uniform int GrnU;
 uniform int BlueU;


void main()
{
    // FragColor = ivec3(Red, Grn, Blue); // Does not  work 

    FragColor = vec4(RedU/255.0, GrnU/255.0, BlueU/255.0, 1.0); // Hope it does not round off the value

    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}