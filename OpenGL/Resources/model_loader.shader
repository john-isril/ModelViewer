// vertex shader program
#shader vertex
#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 normal;
layout(location = 2) in vec2 texture_coordinates_in;

out vec2 texture_coordinates_out;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	texture_coordinates_out = texture_coordinates_in;
};

#shader fragment
#version 460 core

out vec4 FragColor;
in vec2 texture_coordinates_out;

uniform sampler2D texture_diffuse1;

//uniform sampler2D texture_specular1;

//uniform sampler2D texture_normal1;

//uniform sampler2D texture_height1;

//uniform sampler2D texture_roughness1;

void main()
{
	//vec3 diffuse = vec3(texture(texture_diffuse1, texture_coordinates_out));
	//vec3 specular = vec3(texture(texture_specular1, texture_coordinates_out));
	//vec3 normal = vec3(texture(texture_normal1, texture_coordinates_out));
	//vec3 height = vec3(texture(texture_height1, texture_coordinates_out));
	//vec3 roughness = vec3(texture(texture_roughness1, texture_coordinates_out));
	//vec3 result = diffuse;
	FragColor = texture(texture_diffuse1, texture_coordinates_out);
};