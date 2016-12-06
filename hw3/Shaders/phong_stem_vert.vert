#version 330

layout(location = 3) in vec3 Position;
layout(location = 4) in vec2 texcoord1;
layout(location = 5) in vec3 norm1;

uniform mat4 MV;
uniform mat4 Projection;

out vec3 norm;
out vec2 texcoord;
out vec3 pos;

void main()
{
	gl_Position = Projection*MV*vec4(Position, 1.0);
	texcoord = texcoord1;
	norm = norm1;
	pos = Position;
}