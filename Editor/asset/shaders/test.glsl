#shadertype vertex
#version 450 core
layout(location = 0) in vec3 a_pos;
layout(location =1)  in vec4 a_color;
layout(location =2)  in vec2 a_texcoord;
layout(location =3)  in float a_texIndex;
layout(location =4)  in int a_entityID;

layout(std140, binding = 0) uniform camera
{
	mat4 u_viewProjection;
};

layout(location=0) out vec4 v_color;
layout(location=2) out vec2 v_texcoord;
layout(location=3) out flat float v_texIndex;
layout(location=4) out flat int   v_entityID;
void main()
{
	v_color = a_color;
	v_texcoord=a_texcoord;
	v_texIndex=a_texIndex;
	v_entityID=a_entityID;
	gl_Position=u_viewProjection*vec4(a_pos,1.0);
}

#shadertype fragment
#version 450 core
layout(location = 0) out vec4 color;
layout(location = 1) out int color1;

layout(binding =0 ) uniform sampler2D u_textures[32];

layout(location=0) in vec4 v_color;
layout(location=2) in vec2 v_texcoord;
layout(location=3) in flat float v_texIndex;
layout(location=4) in flat int   v_entityID;
void main()
{
	color = v_color;
	switch(int(v_texIndex))
	{
		case  0: color *= texture(u_textures[ 0], v_texcoord); break;
		case  1: color *= texture(u_textures[ 1], v_texcoord); break;
		case  2: color *= texture(u_textures[ 2], v_texcoord); break;
		case  3: color *= texture(u_textures[ 3], v_texcoord); break;
		case  4: color *= texture(u_textures[ 4], v_texcoord); break;
		case  5: color *= texture(u_textures[ 5], v_texcoord); break;
		case  6: color *= texture(u_textures[ 6], v_texcoord); break;
		case  7: color *= texture(u_textures[ 7], v_texcoord); break;
		case  8: color *= texture(u_textures[ 8], v_texcoord); break;
		case  9: color *= texture(u_textures[ 9], v_texcoord); break;
		case 10: color *= texture(u_textures[10], v_texcoord); break;
		case 11: color *= texture(u_textures[11], v_texcoord); break;
		case 12: color *= texture(u_textures[12], v_texcoord); break;
		case 13: color *= texture(u_textures[13], v_texcoord); break;
		case 14: color *= texture(u_textures[14], v_texcoord); break;
		case 15: color *= texture(u_textures[15], v_texcoord); break;
		case 16: color *= texture(u_textures[16], v_texcoord); break;
		case 17: color *= texture(u_textures[17], v_texcoord); break;
		case 18: color *= texture(u_textures[18], v_texcoord); break;
		case 19: color *= texture(u_textures[19], v_texcoord); break;
		case 20: color *= texture(u_textures[20], v_texcoord); break;
		case 21: color *= texture(u_textures[21], v_texcoord); break;
		case 22: color *= texture(u_textures[22], v_texcoord); break;
		case 23: color *= texture(u_textures[23], v_texcoord); break;
		case 24: color *= texture(u_textures[24], v_texcoord); break;
		case 25: color *= texture(u_textures[25], v_texcoord); break;
		case 26: color *= texture(u_textures[26], v_texcoord); break;
		case 27: color *= texture(u_textures[27], v_texcoord); break;
		case 28: color *= texture(u_textures[28], v_texcoord); break;
		case 29: color *= texture(u_textures[29], v_texcoord); break;
		case 30: color *= texture(u_textures[30], v_texcoord); break;
		case 31: color *= texture(u_textures[31], v_texcoord); break;
	}
	color1=v_entityID;
}