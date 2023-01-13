#shader vertex
#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texture_coordinates_in;

out vec2 texture_coordinates;

void main()
{
    texture_coordinates = texture_coordinates_in;
    gl_Position = vec4(position.xy, 0.0, 1.0);
}

#shader fragment
#version 460 core

out vec4 FragColor;

in vec2 texture_coordinates;

uniform sampler2D screen_quad_texture;
uniform int filter_type;
uniform float vignette_intensity;
uniform float time;
uniform float blur_intensity;

void main()
{
    const float inner_vignette_radius = 0.1;
    const float outer_vignette_radius = 1.0;
    const float blur_direction = 16.0;
    const float TAU = 6.28;

    vec4 texture_color = vec4(texture(screen_quad_texture, texture_coordinates).rgb, 1.0);
    vec4 color = vec4(0.0);

    switch (filter_type) 
    {
    case 0:
        color = texture_color;
        break;

    case 1:
        color = vec4(vec3(1.0 - texture(screen_quad_texture, texture_coordinates)), 1.0);
        break;

    case 2:
        float average = (texture_color.r + texture_color.g + texture_color.b) / 3.0;
        color = vec4(average, average, average, 1.0);
        break;

    case 3:
        vec2 centered = texture_coordinates - vec2(0.5);
        vec4 final_color = vec4(1.0);
        final_color.rgb *= 1.0 - smoothstep(inner_vignette_radius, outer_vignette_radius, length(centered));
        final_color *= texture_color;

        color = mix(texture_color, final_color, vignette_intensity);
        break;

    case 4:
        float cosine_time = cos(time);
        float blur = blur_intensity + blur_intensity * cosine_time;
        vec2 radius = blur / texture_coordinates;

        for (float i = 0.0; i < TAU; i += TAU / blur_direction)
        {
            for (float j = 1.0 / 3.0; j <= 1.0; j += 1.0 / 3.0)
            {
                texture_color += texture(screen_quad_texture, texture_coordinates + vec2(cos(i), sin(i)) * radius * j);
            }
        }

        texture_color /= (3.0 * blur_direction) - 15.0;

        if (texture_coordinates.x < 0.5 + 0.5 * cosine_time)
        {
            float greyVal = (texture_color.x + texture_color.y + texture_color.z) / 3.0;
            color = vec4(greyVal, greyVal, greyVal, 1.0);
        }
        else
        {
            color = texture_color;
        }

        break;

    default:
        color = vec4(texture(screen_quad_texture, texture_coordinates).rgb, 1.0);
        break;
    }

    FragColor = vec4(pow(color.rgb, vec3(1.0 / 2.2)), 1.0);
}