#version 330 core

in vec3 outColor;
in vec2 TexCoords;

out vec4 FragColor;

uniform float mixAmount;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, TexCoords), texture(texture2, vec2(TexCoords.x, 1 - TexCoords.y)), mixAmount);
}
