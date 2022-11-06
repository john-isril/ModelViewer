#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "Shader.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "FrameBuffer.h"
#include "Window.h"
#include "Camera.h"

#define TRANSFORM_TRANSLATION 0b1
#define TRANSFORM_ROTATION 0b10
#define TRANSFORM_SCALE 0b100
#define TRANSFORM_SCALE_UNIFORM 0b1000

class Editor
{
public:
	Editor(const Editor&) = delete;
	Editor& operator=(const Editor&) = delete;
	Editor(Editor&&) = delete;
	Editor& operator=(Editor&&) = delete;

	static void Init(GLFWwindow* window, const char* GLSL_version);
	static void BeginRender();
	static void EndRender();
	static void Shutdown();

	static void CreateTransformMenu(const char* title, Transform* transform, uint8_t transform_flags);
	static void CreatePointLightMenu(const char* title, PointLight* point_light);
	static void CreateDirectionalLightMenu(const char* title, DirectionalLight* directional_light);
	static void CreateFiltersMenu(const char* title, Shader& shader, float time);
	static void CreateBackgroundMenu(const char* title, glm::vec3& color);
	static void CreateTextInput(const char* title, class Model* model);
	static void CreateViewWindow(const char* title, glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer);
	static bool ViewportSelected();
	static bool ViewportHovered();
	static bool ShowSkybox();

	~Editor();

private:
	Editor();
	static Editor m_instance;
	int m_filter_type;
	float m_vignette_intensity;
	float m_blur_intensity;
	bool m_show_skybox;
	bool viewport_selected;
	bool viewport_hovered;
	static Editor& Get();

	static void InitImpl(GLFWwindow* window, const char* GLSL_version);
	static void BeginRenderImpl();
	static void EndRenderImpl();
	static void ShutdownImpl();

	static void CreateTransformMenuImpl(const char* title, Transform* transform, uint8_t transform_flags);
	static void CreatePointLightMenuImpl(const char* title, PointLight* point_light);
	static void CreateDirectionalLightMenuImpl(const char* title, DirectionalLight* directional_light);
	static void CreateFiltersMenuImpl(const char* title, Shader& shader, float time);
	static void CreateBackgroundMenuImpl(const char* title, glm::vec3& color);
	static void CreateTextInputImpl(const char* title, class Model* model);
	static void CreateViewWindowImpl(const char* title, glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer);
	static bool ViewportSelectedImpl();
	static bool ViewportHoveredImpl();
	static bool ShowSkyboxImpl();

	static void ToggleSkybox();
};