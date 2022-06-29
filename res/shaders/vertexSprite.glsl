#version 460

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 textureCoord;

out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
   texCoord = textureCoord;
   gl_Position = projectionMatrix * modelMatrix * vec4(vertexPosition, 0.0, 1.0);
}