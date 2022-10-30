// vertex shader program
#shader vertex
#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texture_coordinates_in;

uniform mat4 mvp;
uniform float outline_scale;

void main()
{
	gl_Position = mvp * vec4(position + normal_in * outline_scale, 1.0);
};

#shader fragment
#version 460 core

out vec4 FragColor;
uniform vec3 outline_color;

void main()
{
	FragColor = vec4(outline_color, 1.0);
};