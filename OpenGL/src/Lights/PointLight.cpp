#include "PointLight.h"

PointLight::PointLight(const std::string& path, const std::string_view& name, float brightness, const glm::vec4& color, const glm::vec4& specular_color, bool is_on, float constant, float quadratic, float linear, bool is_hidden) :
	Light(name, brightness, color, specular_color, is_on),
	m_model{path}, m_constant{constant}, m_quadratic{quadratic}, m_linear{linear}, m_is_hidden{is_hidden}
{
	m_model.GetTransform().SetTranslation(glm::vec3{ 1.2f, 1.0f, 2.0f });
	m_model.GetTransform().SetScale(glm::vec3{ 0.5f });
}

Model& PointLight::GetModel()
{
	return m_model;
}

float PointLight::GetConstant() const
{
	return m_constant;
}

float PointLight::GetLinear() const
{
	return m_linear;
}

float PointLight::GetQuadratic() const
{
	return m_quadratic;
}

bool& PointLight::GetIsHidden()
{
	return m_is_hidden;
}

void PointLight::SetConstant(float constant)
{
	m_constant = constant;
}

void PointLight::SetLinear(float linear)
{
	m_linear = linear;
}

void PointLight::SetQuadratic(float quadratic)
{
	m_quadratic = quadratic;
}