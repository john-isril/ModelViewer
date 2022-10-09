#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdint>
#include "Transform.h"

class Camera
{
public:
    enum class State : uint8_t
    {
        Rotating,
        Walking,
        Panning,
        None
    };

public:
    Camera(const Transform &transform = {}, glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f),
    float translation_speed = 2.7f, float rotation_sensitivity = 0.1f);
    ~Camera();

    void ProcessMouseMovement(float x, float y, float delta_time);
    void ProcessMouseButtonPress(bool left_is_pressed, bool right_is_pressed);
    void Rotate(float yaw_offset, float pitch_offset);
    void Walk(float x, float y, float delta_time);
    void Pan(float x, float y, float delta_time);
    void LevelOrientation();

    const Camera::State GetState() const;
    const glm::mat4 GetViewMatrix() const;
    const glm::vec3& GetPosition() const;
    const glm::vec3& GetForward() const;

    void SetState(Camera::State state);

private:
    Camera::State m_state;
    float m_translation_speed;
    float m_rotation_sensitivity;
    float last_mouse_x_pos;
    float last_mouse_y_pos;
    bool first_mouse;

    //glm::vec3 m_position;
    Transform m_transform;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    //Rotation m_rotation;

private:
    void UpdateVectors();
    void CheckPitchConstraint();
};