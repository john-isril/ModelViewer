#shader vertex
#version 460 core

layout(location = 0) in vec3 position;

out vec3 texture_coordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texture_coordinates = position;
    vec4 skybox_position = projection * view * vec4(position, 1.0);
    gl_Position = skybox_position.xyww;
}

#shader fragment
#version 460 core

out vec4 FragColor;
in vec3 texture_coordinates;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, texture_coordinates);
}