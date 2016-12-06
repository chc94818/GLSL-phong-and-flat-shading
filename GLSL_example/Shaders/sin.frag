#version 330

in vec2 position_v;

out vec4 outColor;

void main() {
	outColor = vec4(1.0, position_v, 1.0);
}