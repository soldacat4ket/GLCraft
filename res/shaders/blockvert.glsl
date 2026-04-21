#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;

uniform mat4 u_MVP;

out vec2 VertTexCord;

void main()
{
    VertTexCord = aTex;
    gl_Position = u_MVP * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0);
}