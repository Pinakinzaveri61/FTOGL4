#version 400 core

out vec4 FragColor;

// Color per Shape, i.e. 'ourColor' set before drawing the shape and is applied to all the vertices of the shape.
uniform vec4 ourColor;

void main()
{
     FragColor = vec4(ourColor);
    //FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}