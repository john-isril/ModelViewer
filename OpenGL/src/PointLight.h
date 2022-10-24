#pragma once
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class PointLight
{
public:
	PointLight(const std::string &path);

	Model& GetModel();
	const glm::vec3& GetSpecular() const;
	glm::vec3& GetColor();
	const glm::vec3& GetDiffuse() const;
	const glm::vec3& GetAmbient() const;
	float& GetBrightness();
	float GetConstant() const;
	float GetLinear() const;
	float GetQuadratic() const;
	bool& GetIsOn();
	bool& GetIsHidden();

	void SetSpecular(const glm::vec3 &specular);
	void SetColor(const glm::vec3& color);
	void SetDiffuse(const glm::vec3& diffuse);
	void SetAmbient(const glm::vec3& ambient);
	void SetBrightness(float brightness);
	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);
	void UpdateColors();

private:
	Model m_model;
	glm::vec3 m_specular;
	glm::vec3 m_color;
	glm::vec3 m_diffuse_color;
	glm::vec3 m_ambient_color;
	float m_brightness;
	float m_constant;
	float m_linear;
	float m_quadratic;
	bool m_is_on;
	bool m_is_hidden;
};