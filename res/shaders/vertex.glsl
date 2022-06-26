#version 460

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 textureCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
   color = vertexColor;
   texCoord = textureCoord;
   gl_Position = projectionMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}