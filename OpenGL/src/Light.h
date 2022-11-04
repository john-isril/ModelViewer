#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	Light();

	glm::vec4& GetColor();
	const glm::vec4& GetDiffuse() const;
	const glm::vec4& GetSpecular() const;
	const glm::vec4& GetAmbient() const;
	float& GetBrightness();
	float GetBrightnessOffOn() const;
	bool& GetIsOn();

	void SetColor(const glm::vec4& color);
	void SetDiffuse(const glm::vec4& diffuse);
	void SetSpecular(const glm::vec4& specular);
	void SetAmbient(const glm::vec4& ambient);
	void SetBrightness(float brightness);
	void UpdateColors();
	void Toggle();

protected:
	float m_brightness;
	glm::vec4 m_color;
	glm::vec4 m_diffuse_color;
	glm::vec4 m_specular_color;
	glm::vec4 m_ambient_color;
	bool m_is_on;
};