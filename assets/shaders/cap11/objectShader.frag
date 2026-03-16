#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 ambientColor;

void main() {
  FragColor = vec4(vec3(lightColor * ambientColor), 1.0);
}
