// vertex shader program
#shader vertex
#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal_in;
layout(location = 2) in vec2 texture_coordinates_in;

uniform mat4 model;
uniform mat4 mvp;
out vec2 texture_coordinates;
out vec3 fragment_position;
out vec3 normal;

void main()
{
	gl_Position = mvp * vec4(position, 1.0);
	texture_coordinates = texture_coordinates_in;
	fragment_position = vec3(model * vec4(position, 1.0)); // coverts the position to world coordinates
	// todo: figure out how to move this to cpu
	normal = mat3(transpose(inverse(model))) * normal_in;
};

#shader fragment
#version 460 core

struct Material
{
	float shininess;
	sampler2D diffuse1;
	sampler2D specular1;
	sampler2D normal1;
	sampler2D emission;
	// dont need ambient bc same as diffuse
	//vec3 ambient;
};

struct DirectionalLight
{
	float brightness;

	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	float brightness;
	float constant;
	float linear_;
	float quadratic;

	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	float brightness;
	float constant;
	float linear_;
	float quadratic;

	float cutoff;
	float outer_cutoff;

	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

out vec4 FragColor;

uniform Material material;
uniform DirectionalLight directional_light;
uniform PointLight point_light;
uniform SpotLight spot_light;
uniform vec3 object_color;
uniform vec3 view_position;

// function prototypes
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_direction);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction);

void main()
{

	vec3 object_normal = normalize(normal);
	vec3 object_to_view_direction = normalize(view_position - fragment_position);

	vec3 result = vec3(0.0, 0.0, 0.0);
	result += CalcDirectionalLight(directional_light, object_normal, object_to_view_direction);
	result += CalcPointLight(point_light, object_normal, fragment_position, object_to_view_direction);

	FragColor = vec4(result, 1.0);
};

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_direction)
{
	vec3 object_to_light_direction = normalize(-light.direction);

	vec3 ambient_light = light.ambient * vec3(texture(material.diffuse1, texture_coordinates));

	float diffuse_strength = max(dot(normal, object_to_light_direction), 0.0);
	vec3 diffuse_light = light.diffuse * vec3(texture(material.diffuse1, texture_coordinates)) * diffuse_strength;

	vec3 light_reflection_direction = reflect(object_to_light_direction, normal);
	float specular_strength = pow(max(dot(view_direction, light_reflection_direction), 0.0), material.shininess);
	vec3 specular_light = specular_strength * light.specular * vec3(texture(material.specular1, texture_coordinates));

	return ambient_light + (light.brightness * (diffuse_light + specular_light));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragment_position, vec3 view_direction)
{
	vec3 object_to_light_direction = normalize(light.position - fragment_position);

	// ambient
	vec3 ambient_light = light.ambient * vec3(texture(material.diffuse1, texture_coordinates));

	// diffuse
	float diffuse_strength = max(dot(normal, object_to_light_direction), 0.0);
	vec3 diffuse_light = light.diffuse * diffuse_strength * vec3(texture(material.diffuse1, texture_coordinates));

	// specular
	vec3 light_reflection_direction = reflect(-object_to_light_direction, normal);
	float specular_strength = pow(max(dot(view_direction, light_reflection_direction), 0.0), material.shininess);

	vec3 specular_light = specular_strength * light.specular * vec3(texture(material.specular1, texture_coordinates));

	float distance = length(light.position - fragment_position);
	float attenuation = 1.0 / (light.constant + light.linear_ * distance + light.quadratic * (distance * distance));

	return (light.brightness * attenuation * (ambient_light + diffuse_light + specular_light));
}