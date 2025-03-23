#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 pos;

void main()
{
    vec3 translated = aPos + pos;
    gl_Position = projection * view * vec4(translated, 1.0);
}