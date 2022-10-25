#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& direction) :
	Light(), m_direction{ direction }
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
