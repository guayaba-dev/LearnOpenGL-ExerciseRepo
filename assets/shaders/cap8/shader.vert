#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 MVP;

out vec3 outColor;
out vec2 TexCoords;

void main()
{
  outColor = aColor;
  TexCoords = aTexCoords;
  gl_Position = MVP * vec4(aPos, 1.0);
}
