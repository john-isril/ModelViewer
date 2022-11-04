#include "Camera.h"

Camera::Camera(const Transform& transform, glm::vec3 world_up,
    float translation_speed, float rotation_sensitivity, float field_of_view, float near_plane_distance, float far_plane_distance) :
    m_state{ Camera::State::None }, m_translation_speed{ translation_speed }, m_rotation_sensitivity{ rotation_sensitivity }, 
    m_field_of_view{ field_of_view }, m_near_plane_distance{ near_plane_distance }, m_far_plane_distance{far_plane_distance},
    last_mouse_x_pos{}, last_mouse_y_pos{}, first_mouse{ true },
    m_transform{ transform }, m_world_up{ world_up }
{
    this->UpdateVectors();
}

Camera::~Camera()
{
}

void Camera::ProcessMouseMovement(float x_pos, float y_pos, float delta_time)
{
    if (m_state == Camera::State::None)
    {
        return;
    }
    
    if (first_mouse)
    {
        last_mouse_x_pos = x_pos;
        last_mouse_y_pos = y_pos;

        first_mouse = false;
    }
    
    const float x_offset { x_pos - last_mouse_x_pos };
    const float y_offset{ last_mouse_y_pos - y_pos };
    last_mouse_x_pos = x_pos;
    last_mouse_y_pos = y_pos;

    switch (m_state)
    {
    case Camera::State::Walking:
        this->Walk(x_offset, y_offset, delta_time);
        break;

    case Camera::State::Rotating:
        this->Rotate(x_offset, y_offset);
        break;

    case Camera::State::Panning:
        this->Pan(x_offset, y_offset, delta_time);
        break;

    default:
        break;
    }
}

void Camera::ProcessMouseButtonPress(bool left_is_pressed, bool right_is_pressed)
{
    if (left_is_pressed && right_is_pressed)
    {
        m_state = Camera::State::Panning;
    }
    else if (left_is_pressed)
    {
        m_state = Camera::State::Walking;
    }
    else if (right_is_pressed)
    {
        m_state = Camera::State::Rotating;
    }
    else
    {
        m_state = Camera::State::None;
        first_mouse = true;
    }
}

void Camera::Rotate(float yaw_offset, float pitch_offset)
{
    m_transform.IncreaseYaw(yaw_offset * m_rotation_sensitivity);
    m_transform.IncreasePitch(pitch_offset * m_rotation_sensitivity);

    CheckPitchConstraint();
    UpdateVectors();
}

void Camera::Walk(float x, float z, float delta_time)
{
    float fwd_magnitude{ z * m_translation_speed * m_rotation_sensitivity * delta_time };
    float right_magnitude{ x * m_translation_speed * m_rotation_sensitivity * delta_time };

    m_transform.AddTranslation(m_forward * fwd_magnitude);
    m_transform.AddTranslation(glm::normalize(glm::cross(m_forward, m_up)) * right_magnitude);
}

void Camera::Pan(float x, float y, float delta_time)
{
    float right_magnitude{ x * m_translation_speed * m_rotation_sensitivity * delta_time };
    float up_magnitude{ y * m_translation_speed * m_rotation_sensitivity * delta_time };

    m_transform.AddTranslation(m_up * up_magnitude);
    m_transform.AddTranslation(glm::normalize(glm::cross(m_forward, m_up)) * right_magnitude);
}

void Camera::LevelOrientation()
{
    m_transform.SetPitch(0.0f);
    m_transform.SetYaw(-90.0f);
    UpdateVectors();
}

const Camera::State Camera::GetState() const
{
    return m_state;
}

const glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(m_transform.GetTranslation(), m_transform.GetTranslation() + m_forward, m_up);
}

const glm::vec3& Camera::GetPosition() const
{
    return m_transform.GetTranslation();
}

const glm::vec3& Camera::GetForward() const
{
    return m_forward;
}

float Camera::GetFieldOfView() const
{
    return m_field_of_view;
}

float Camera::GetFarPlaneDistance() const
{
    return m_far_plane_distance;
}

float Camera::GetNearPlaneDistance() const
{
    return m_near_plane_distance;
}

void Camera::SetState(Camera::State state)
{
    m_state = state;
}

void Camera::SetFieldOfView(float field_of_view)
{
    m_field_of_view = field_of_view;
}

void Camera::SetNearPlaneDistance(float near_plane_distance)
{
    m_near_plane_distance = near_plane_distance;
}

void Camera::SetFarPlaneDistance(float far_plane_distance)
{
    m_far_plane_distance = far_plane_distance;
}

void Camera::UpdateVectors()
{
    m_forward.x = cos(glm::radians(m_transform.GetYaw())) * cos(glm::radians(m_transform.GetPitch()));
    m_forward.y = sin(glm::radians(m_transform.GetPitch()));
    m_forward.z = sin(glm::radians(m_transform.GetYaw())) * cos(glm::radians(m_transform.GetPitch()));
    m_forward = glm::normalize(m_forward);

    m_right = glm::normalize(glm::cross(m_forward, m_world_up));
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}

void Camera::CheckPitchConstraint()
{
    if (m_transform.GetPitch() > 89.0f)
    {
        m_transform.SetPitch(89.0f);
    }

    if (m_transform.GetPitch() < -89.0f)
    {
        m_transform.SetPitch(-89.0f);
    }
}