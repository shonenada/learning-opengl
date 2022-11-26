#version 330 core

out vec4 FragColor;

in vec3 outColor;
in vec2 outTex;

uniform sampler2D outTexture;

void main() {
    FragColor = texture(outTexture, outTex) * vec4(outColor, 1.0);
}
