#version 330

layout(location = 0) in vec2 Position;

out vec2 position_v;

void main() {
	vec2 p = vec2(Position.x, sin(Position.x*2));
	gl_Position = vec4(p, 0.0, 1.0);
	position_v = p;
}