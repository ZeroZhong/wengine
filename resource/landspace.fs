#version 330 core

in vec3 OurColor;

out vec4 FragColor;

void main()
{
  FragColor = vec4(OurColor.x, OurColor.y, OurColor.z, 1.0f);
}