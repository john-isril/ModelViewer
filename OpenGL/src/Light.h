#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	Light();

	glm::vec3& GetColor();
	const glm::vec3& GetDiffuse() const;
	const glm::vec3& GetSpecular() const;
	const glm::vec3& GetAmbient() const;
	float& GetBrightness();
	float GetBrightnessOffOn() const;
	bool& GetIsOn();

	void SetColor(const glm::vec3& color);
	void SetDiffuse(const glm::vec3& diffuse);
	void SetSpecular(const glm::vec3& specular);
	void SetAmbient(const glm::vec3& ambient);
	void SetBrightness(float brightness);
	void UpdateColors();
	void Toggle();

protected:
	float m_brightness;
	glm::vec3 m_color;
	glm::vec3 m_diffuse_color;
	glm::vec3 m_specular_color;
	glm::vec3 m_ambient_color;
	bool m_is_on;
};