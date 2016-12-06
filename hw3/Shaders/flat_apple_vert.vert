#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 texcoord1;

uniform mat4 MV;
uniform mat4 Projection;
out vec2 texcoord;
out vec3 pos;

void main()
{
	gl_Position = Projection*MV*vec4(Position, 1.0);
	texcoord = texcoord1;
	pos = Position;
}