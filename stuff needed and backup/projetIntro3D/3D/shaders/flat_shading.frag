#version 330

in vec3 color;

uniform float curOpa;

layout(location = 0) out vec4 final_col;

void main()
{
	// BCOR
	final_col = vec4(color,curOpa);
}
