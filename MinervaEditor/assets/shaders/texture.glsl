#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_tilingFactor;

uniform mat4 u_VP;

out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;
out float v_tilingFactor;

void main()
{
	v_color = a_color;
	v_texCoord = a_texCoord;
	v_texIndex = a_texIndex;
	v_tilingFactor = a_tilingFactor;
	gl_Position = u_VP * vec4(a_position, 1.0);	
}



#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

uniform sampler2D u_textures[8];

in vec4 v_color;
in vec2 v_texCoord;
in float v_texIndex;
in float v_tilingFactor;

void main()
{
	// When aggregated into arrays within a shader, they can only be indexed with a
	// dynamically uniform integral expression, otherwise results are undefined.
	vec4 texColor;
	switch(int(v_texIndex))
	{
		case 0: texColor = texture(u_textures[0], v_texCoord * v_tilingFactor); break;
		case 1: texColor = texture(u_textures[1], v_texCoord * v_tilingFactor); break;
		case 2: texColor = texture(u_textures[2], v_texCoord * v_tilingFactor); break;
		case 3: texColor = texture(u_textures[3], v_texCoord * v_tilingFactor); break;
		case 4: texColor = texture(u_textures[4], v_texCoord * v_tilingFactor); break;
		case 5: texColor = texture(u_textures[5], v_texCoord * v_tilingFactor); break;
		case 6: texColor = texture(u_textures[6], v_texCoord * v_tilingFactor); break;
		case 7: texColor = texture(u_textures[7], v_texCoord * v_tilingFactor); break;
		default: texColor = vec4(1.0);
	}
	color = texColor*v_color;
}