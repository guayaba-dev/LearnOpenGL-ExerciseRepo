#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2);
}
