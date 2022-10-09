// vertex shader program
#shader vertex
#version 460 core
layout (location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec2 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;
out vec2 TexCoord;

void main()
{
   gl_Position = projection * view * model * vec4(position, 1.0);
   TexCoord = vec2(tex_coord.x, tex_coord.y);
};

#shader fragment
#version 460 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture_1;

void main()
{
	FragColor = texture(texture_1, TexCoord);
};