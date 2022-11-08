#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const std::string_view& name, const glm::vec3& direction, float brightness, const glm::vec4& color, const glm::vec4& specular_color, bool is_on) :
	Light(name, brightness, color, specular_color, is_on), m_direction{ direction }
{
}

glm::vec3& DirectionalLight::GetDirection()
{
	return m_direction;
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
	m_direction = direction;
}
