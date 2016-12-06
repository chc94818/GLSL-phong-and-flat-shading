#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 texcoord1;
layout(location = 2) in vec3 norm1;

uniform mat4 MV;
uniform mat4 Projection;



out vec3 c;
out vec3 vertex_view_space;
out vec2 texcoord;
out vec3 pos;

void main()
{
	c = vec3(MV * vec4(0.0,0.0,0.0,1.0));
	gl_Position = Projection*MV*vec4(Position, 1.0);
	texcoord = texcoord1;
	pos = Position;
	vertex_view_space = (MV * vec4(Position,1.0)).xyz;
}