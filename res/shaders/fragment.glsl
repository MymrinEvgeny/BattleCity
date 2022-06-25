#version 460

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex;

void main() {
   gl_FragColor = texture(tex, texCoord);
}