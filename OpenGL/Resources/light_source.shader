// vertex shader program
#shader vertex
#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
};

#shader fragment
#version 460 core

out vec4 FragColor;

uniform vec3 light_color;

void main()
{
	FragColor = vec4(light_color, 1.0);
};