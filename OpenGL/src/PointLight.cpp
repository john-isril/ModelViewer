#include "PointLight.h"

PointLight::PointLight(const std::string& path) :
	m_model{path}, m_specular{1.0f}, m_color{1.0f}, m_constant{1.0f}, m_quadratic{0.032f}, m_is_on{false}, m_brightness{1.0f}, m_linear{0.09}, m_is_hidden{false}
{
	m_model.GetTransform().SetTranslation(glm::vec3{ 1.2f, 1.0f, 2.0f });
	m_model.GetTransform().SetScale(glm::vec3{ 0.5f });
}

Model& PointLight::GetModel()
{
	return m_model;
}

const glm::vec3& PointLight::GetSpecular() const
{
	return m_specular;
}

glm::vec3& PointLight::GetColor()
{
	return m_color;
}

const glm::vec3& PointLight::GetDiffuse() const
{
	return m_diffuse_color;
}

const glm::vec3& PointLight::GetAmbient() const
{
	return m_ambient_color;
}

float& PointLight::GetBrightness()
{
	return m_brightness;
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

bool& PointLight::GetIsOn()
{
	return m_is_on;
}

bool& PointLight::GetIsHidden()
{
	return m_is_hidden;
}

void PointLight::SetSpecular(const glm::vec3& specular)
{
	m_specular = specular;
}

void PointLight::SetColor(const glm::vec3& color)
{
	m_color = color;
}

void PointLight::SetDiffuse(const glm::vec3& diffuse)
{
	m_diffuse_color = diffuse;
}

void PointLight::SetAmbient(const glm::vec3& ambient)
{
	m_ambient_color = ambient;
}

void PointLight::SetBrightness(float brightness)
{
	m_brightness = brightness;
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

void PointLight::UpdateColors()
{
	m_diffuse_color = m_color * glm::vec3(0.5f);
	m_ambient_color = m_color * glm::vec3(0.2f);
}
