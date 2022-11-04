// vertex shader program
#shader vertex
#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texture_coordinates_in;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
};

#shader fragment
#version 460 core

out vec4 FragColor;

uniform vec4 light_color;
uniform float light_brightness;

void main()
{
	FragColor = light_brightness * light_color;
};

