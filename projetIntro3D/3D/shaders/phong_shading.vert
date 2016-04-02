#version 330

layout(location=0) in vec3 vx_pos; // Indice 0
layout(location=1) in vec3 vx_nrm; // Indice 1
layout(location=3) in vec3 vx_col; // Indice 3

uniform mat4 projectionMat;
uniform mat4 modelviewMat;
uniform mat4 viewMat;
uniform mat4 normalMat;

out vec3 color;
out vec3 normal;	// In camera frame
out vec3 pos;		// In camera frame

void main()
{
	gl_Position = projectionMat*modelviewMat*vec4(vx_pos,1.0);

	color = vx_col;
	normal = vx_nrm;

}
