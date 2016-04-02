#version 330

layout(location=0) in vec3 vx_pos; // Indice 0
layout(location=3) in vec3 vx_col; // Indice 3

uniform mat4 projectionMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform float temps;

out vec3 color;

void main()
{
	vec3 coord = vx_pos;
	coord.z += 50*temps;
	gl_Position = projectionMat*viewMat*modelMat*vec4(coord,1.0);	
	color = vx_col;
}
