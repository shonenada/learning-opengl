#version 330 core

out vec4 FragColor;

in vec3 outColor;
in vec2 outTex;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    FragColor = texture(texture1, outTex) * vec4(outColor, 1.0);
}
