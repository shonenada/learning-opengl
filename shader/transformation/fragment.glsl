#version 330 core

out vec4 FragColor;

in vec3 outColor;
in vec2 outTex;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    FragColor = mix(texture(texture1, outTex), texture(texture2, outTex), 0.2) * vec4(outColor, 1.0);
}
