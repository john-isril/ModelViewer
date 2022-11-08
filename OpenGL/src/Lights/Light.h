#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string_view>
#include <string>

class Light
{
public:
	Light(const std::string_view& name = "Light", float brightness = 1.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0, 1.0f }, const glm::vec4& specular_color = { 1.0f, 1.0f, 1.0, 1.0f }, bool is_on = true);

	glm::vec4& GetColor();
	const glm::vec4& GetDiffuse() const;
	const glm::vec4& GetSpecular() const;
	const glm::vec4& GetAmbient() const;
	float& GetBrightness();
	float GetBrightnessOffOn() const;
	bool& GetIsOn();
	const std::string GetName() const;

	void SetColor(const glm::vec4& color);
	void SetDiffuse(const glm::vec4& diffuse);
	void SetSpecular(const glm::vec4& specular);
	void SetAmbient(const glm::vec4& ambient);
	void SetBrightness(float brightness);
	void SetName(const std::string& name);
	void UpdateColors();
	void Toggle();

protected:
	float m_brightness;
	glm::vec4 m_color;
	glm::vec4 m_diffuse_color;
	glm::vec4 m_specular_color;
	glm::vec4 m_ambient_color;
	bool m_is_on;
	std::string m_name;
};