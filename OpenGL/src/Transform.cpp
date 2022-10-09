#include "Transform.h"

Transform::Transform(const glm::vec3& scale, const Rotation& rotation, const glm::vec3& translation) :
	m_scale{scale}, m_rotation{rotation}, m_translation{translation}
{
}

float Transform::GetScaleX() const
{
	return m_scale.x;
}

float Transform::GetScaleY() const
{
	return m_scale.y;
}

float Transform::GetScaleZ() const
{
	return m_scale.z;
}

const glm::vec3& Transform::GetScale() const
{
	return m_scale;
}

void Transform::SetScale(const glm::vec3 scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;
}

void Transform::SetScaleX(float x)
{
	m_scale.x = x;
}

void Transform::SetScaleY(float y)
{
	m_scale.y = y;
}

void Transform::SetScaleZ(float z)
{
	m_scale.z = z;
}

void Transform::SetScaleUniform(float scale)
{
	m_scale.x = scale;
	m_scale.y = scale;
	m_scale.z = scale;
}

void Transform::IncreaseScaleX(float increment)
{
	m_scale.x += increment;
}

void Transform::IncreaseScaleY(float increment)
{
	m_scale.y += increment;
}

void Transform::IncreaseScaleZ(float increment)
{
	m_scale.z += increment;
}

void Transform::IncreaseScaleUniform(float increment)
{
	m_scale.x += increment;
	m_scale.y += increment;
	m_scale += increment;
}

float Transform::GetYaw() const
{
	return m_rotation.yaw;
}

float Transform::GetPitch() const
{
	return m_rotation.pitch;
}

float Transform::GetRoll() const
{
	return m_rotation.roll;
}

const Rotation& Transform::GetRotation() const
{
	return m_rotation;
}

void Transform::SetYaw(float yaw)
{
	m_rotation.yaw = yaw;
}

void Transform::SetPitch(float pitch)
{
	m_rotation.pitch = pitch;
}

void Transform::SetRoll(float roll)
{
	m_rotation.roll = roll;
}

void Transform::SetRotation(const Rotation& rotation)
{
	m_rotation.yaw = rotation.yaw;
	m_rotation.pitch = rotation.pitch;
	m_rotation.roll = rotation.roll;
}

void Transform::IncreaseYaw(float increment)
{
	m_rotation.yaw += increment;
}

void Transform::IncreasePitch(float increment)
{
	m_rotation.pitch += increment;
}

void Transform::IncreaseRoll(float increment)
{
	m_rotation.roll += increment;
}

void Transform::DecreaseYaw(float decrement)
{
	m_rotation.yaw -= decrement;
}

void Transform::DecreasePitch(float decrement)
{
	m_rotation.pitch -= decrement;
}

void Transform::DecreaseRoll(float decrement)
{
	m_rotation.roll -= decrement;
}

float Transform::GetTranslationX() const
{
	return m_translation.x;
}

float Transform::GetTranslationY() const
{
	return m_translation.y;
}

float Transform::GetTranslationZ() const
{
	return m_translation.z;
}

const glm::vec3& Transform::GetTranslation() const
{
	return m_translation;
}

void Transform::SetTranslationX(float x)
{
	m_translation.x = x;
}

void Transform::SetTranslationY(float y)
{
	m_translation.y = y;
}

void Transform::SetTranslationZ(float z)
{
	m_translation.z = z;
}

void Transform::SetTranslation(const glm::vec3& vec)
{
	m_translation.x = vec.x;
	m_translation.y = vec.y;
	m_translation.z = vec.z;
}

void Transform::IncreaseTranslationX(float increment)
{
	m_translation.x += increment;
}

void Transform::IncreaseTranslationY(float increment)
{
	m_translation.y += increment;
}

void Transform::IncreaseTranslationZ(float increment)
{
	m_translation.z += increment;
}

void Transform::AddTranslation(const glm::vec3& vec)
{
	m_translation += vec;
}

void Transform::SubtractTranslation(const glm::vec3& vec)
{
	m_translation -= vec;
}

Transform::~Transform()
{
}
