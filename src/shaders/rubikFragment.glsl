#version 460 core

in vec3 vColor;
in float vEdge;

out vec4 fragColor;

void main() {
    fragColor = vec4(vColor, 1.0f);
}