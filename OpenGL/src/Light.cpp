#include "Light.h"

Light::Light() :
	m_brightness{1.0f}, m_color{1.0f}, m_specular_color{1.0f}, m_is_on{true}
{
	this->UpdateColors();
}

glm::vec4& Light::GetColor()
{
	return m_color;
}

const glm::vec4& Light::GetDiffuse() const
{
	return m_diffuse_color;
}

const glm::vec4& Light::GetSpecular() const
{
	return m_specular_color;
}

const glm::vec4& Light::GetAmbient() const
{
	return m_ambient_color;
}

float& Light::GetBrightness()
{
	return m_brightness;
}

float Light::GetBrightnessOffOn() const
{
	return (m_is_on) ? m_brightness : 0.0f;
}

bool& Light::GetIsOn()
{
	return m_is_on;
}

void Light::SetColor(const glm::vec4& color)
{
	m_color = color;
}

void Light::SetDiffuse(const glm::vec4& diffuse)
{
	m_diffuse_color = diffuse;
}

void Light::SetSpecular(const glm::vec4& specular)
{
	m_specular_color = specular;
}

void Light::SetAmbient(const glm::vec4& ambient)
{
	m_ambient_color = ambient;
}

void Light::SetBrightness(float brightness)
{
	m_brightness = brightness;
}

void Light::UpdateColors()
{
	m_diffuse_color = m_color * glm::vec4(0.5f);
	m_color.a = 1.0f;
	m_ambient_color = m_color * glm::vec4(0.2f);
	m_ambient_color.a = 1.0f;
}

void Light::Toggle()
{
	m_is_on = 1 - m_is_on;
}
