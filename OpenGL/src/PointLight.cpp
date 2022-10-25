#include "PointLight.h"

PointLight::PointLight(const std::string& path) :
	Light(),
	m_model{path}, m_constant{1.0f}, m_quadratic{0.032f}, m_linear{0.09}, m_is_hidden{false}
{
	m_is_on = false;
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