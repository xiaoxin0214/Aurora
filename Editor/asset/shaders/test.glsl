#shadertype vertex
#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location =1)  in vec4 a_color;
layout(location =2)  in vec2 a_texcoord;
layout(location =3)  in float a_texIndex;
uniform mat4 u_viewProjection;
uniform mat4 u_modelMatrix;
out vec3 v_pos;
out vec4 v_color;
out vec2 v_texcoord;
out float v_texIndex;
void main()
{
	v_pos=a_pos;
	v_color = a_color;
	v_texcoord=a_texcoord;
	v_texIndex=a_texIndex;
	gl_Position=u_viewProjection*u_modelMatrix*vec4(a_pos,1.0);
}

#shadertype fragment
#version 330 core
layout(location = 0) out vec4 color;
uniform sampler2D u_textures[32];
in vec3 v_pos;
in vec4 v_color;
in vec2 v_texcoord;
in float v_texIndex;
void main()
{
	color=texture(u_textures[int(v_texIndex)],v_texcoord);
	color*=v_color;
}