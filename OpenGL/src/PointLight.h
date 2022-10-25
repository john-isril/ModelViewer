#pragma once
#include "Light.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class PointLight : public Light
{
public:
	PointLight(const std::string &path);

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