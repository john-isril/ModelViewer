#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct Rotation
{
	float pitch{ 0.0f };
	float yaw{ 0.0f };
	float roll{ 0.0f };
} Rotation;

class Transform
{
public:
	Transform(const glm::vec3& scale = { 1.0f, 1.0f, 1.0f }, const Rotation& rotation = {}, const glm::vec3& translation = {});
	Transform(const Transform& transform);

	Transform& operator=(const Transform& rhs);
	float GetScaleX() const;
	float GetScaleY() const;
	float GetScaleZ() const;
	const glm::vec3& GetScale() const;
	
	void SetScale(const glm::vec3 scale);
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);
	void SetScaleUniform(float scale);
	void IncreaseScaleX(float increment);
	void IncreaseScaleY(float increment);
	void IncreaseScaleZ(float increment);
	void IncreaseScaleUniform(float increment);

	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;
	const Rotation& GetRotation() const;

	void SetYaw(float yaw);
	void SetPitch(float pitch);
	void SetRoll(float roll);
	void SetRotation(const Rotation& rotation);
	void IncreaseYaw(float increment);
	void IncreasePitch(float increment);
	void IncreaseRoll(float increment);
	void DecreaseYaw(float decrement);
	void DecreasePitch(float decrement);
	void DecreaseRoll(float decrement);
	
	float GetTranslationX() const;
	float GetTranslationY() const;
	float GetTranslationZ() const;
	const glm::vec3& GetTranslation() const;

	void SetTranslationX(float x);
	void SetTranslationY(float y);
	void SetTranslationZ(float z);
	void SetTranslation(const glm::vec3 &vec);
	void IncreaseTranslationX(float increment);
	void IncreaseTranslationY(float increment);
	void IncreaseTranslationZ(float increment);
	void AddTranslation(const glm::vec3& vec); // adding another vector 3 to the position/translation
	void SubtractTranslation(const glm::vec3& vec);

	void UpdateModelMatrix();
	void UpdateMVP(const glm::mat4& view, const glm::mat4& projection);
	const glm::mat4& GetModelMatrix() const;
	const glm::mat4& GetMVPMatrix() const;
	glm::mat4 GetScaledCopyMVP(float scale, const glm::mat4& view, const glm::mat4& projection) const;

	~Transform();

private:
	glm::vec3 m_scale;
	Rotation m_rotation;
	glm::vec3 m_translation;

	glm::mat4 m_model_matrix;
	glm::mat4 m_mvp_matrix;

private:

};