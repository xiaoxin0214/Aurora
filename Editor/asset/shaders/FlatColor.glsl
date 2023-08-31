#shadertype vertex
#version 330 core
layout(location = 0) in vec3 a_pos;
uniform mat4 u_viewProjection;
uniform mat4 u_modelMatrix;
out vec3 v_pos;
void main()
{
	v_pos=a_pos;
	gl_Position=u_modelMatrix*u_viewProjection*vec4(a_pos,1.0);
}

#shadertype fragment
#version 330 core
layout(location = 0) out vec4 color;
uniform vec4 u_color;
in vec3 v_pos;
void main()
{
	color=u_color;
}