#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

uniform mat4 u_VP;
uniform mat4 u_M;

out vec2 v_texCoord;

void main()
{
	v_texCoord = a_texCoord;
	gl_Position = u_VP * u_M * vec4(a_position, 1.0);	
}



#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_color;
uniform float u_tilingFactor;

in vec2 v_texCoord;

void main()
{
	color = texture(u_texture, v_texCoord * u_tilingFactor) * u_color;
}