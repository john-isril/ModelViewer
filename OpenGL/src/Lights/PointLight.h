#pragma once
#include "Light.h"
#include "../Model.h"
#include <string>

class PointLight : public Light
{
public:
	PointLight(const std::string &path = "", const std::string_view& name = "Point Light", float brightness = 1.0f, const glm::vec4& color = { 1.0f, 1.0f, 1.0, 1.0f }, const glm::vec4& specular_color = { 1.0f, 1.0f, 1.0, 1.0f }, bool is_on = true, float constant = 1.0f, float quadratic = 0.032f, float linear = 0.09, bool is_hidden = false);

	Model& GetModel();
	float GetConstant() const;
	float GetLinear() const;
	float GetQuadratic() const;
	bool& GetIsHidden();

	void SetConstant(float constant);
	void SetLinear(float linear);
	void SetQuadratic(float quadratic);

private:
	Model m_model;
	float m_constant;
	float m_linear;
	float m_quadratic;
	bool m_is_hidden;
};