#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const std::string_view &name = "Directional Light", const glm::vec3& direction = {-0.2f, -1.0f, -0.3f}, float brightness = 1.0f, const glm::vec4& color = {1.0f, 1.0f, 1.0, 1.0f}, const glm::vec4& specular_color = {1.0f, 1.0f, 1.0, 1.0f}, bool is_on = true);

	glm::vec3& GetDirection();
	
	void SetDirection(const glm::vec3& direction);

private:
	glm::vec3 m_direction;

};