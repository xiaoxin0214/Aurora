#shadertype vertex
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location =1)  in vec2 a_texcoord;
uniform mat4 u_viewProjection;
uniform mat4 u_modelMatrix;
out vec3 v_pos;
out vec2 v_texcoord;
void main()
{
	v_pos=a_pos;
	v_texcoord=a_texcoord;
	gl_Position=u_viewProjection*u_modelMatrix*vec4(a_pos,1.0);
}

#shadertype fragment
#version 330 core
layout(location = 0) out vec4 color;
uniform vec4 u_color;
uniform sampler2D u_texture;
in vec3 v_pos;
in vec2 v_texcoord;
void main()
{
	color=texture(u_texture,v_texcoord);
	color*=u_color;
}