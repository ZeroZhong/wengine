#version 330 core

layout (location = 0) in vec3 aPos;

uniform vec3 aColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 OurColor;

void main()
{
    OurColor = aColor;
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}