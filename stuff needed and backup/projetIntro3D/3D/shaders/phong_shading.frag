#version 330

in vec3 color;
in vec3 normal;
in vec3 pos;

uniform float curOpa;
uniform vec3 lum_pos;

layout(location = 0) out vec4 final_col;
layout(location = 1) out vec4 unusued_buf;

void main()
{
	// A modifier
	final_col = vec4(color,1.0);
	unusued_buf = vec4(color,1.0);
}
