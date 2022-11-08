#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "Shader.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "FrameBuffer.h"
#include "Window.h"
#include "Camera.h"
#include "Model.h"

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

	static void CreateTransformMenuPanel(const char* title, Transform* transform, uint8_t transform_flags);
	static void CreatePointLightMenuPanel(PointLight* point_light);
	static void CreateDirectionalLightMenuPanel(DirectionalLight* directional_light);
	static void CreateFiltersMenuPanel(Shader& shader, float time);
	static void CreateBackgroundMenuPanel(glm::vec3& color);
	static void CreateModelLoaderPanel(Model* model);
	static void CreateViewport(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer);
	static void GeneratePanels(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer, Model* model, glm::vec3& background_color, Shader &shader, float time,
										DirectionalLight* directional_light, PointLight* point_light);
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

	static void CreateTransformMenuPanelImpl(const char* title, Transform* transform, uint8_t transform_flags);
	static void CreatePointLightMenuPanelImpl(PointLight* point_light);
	static void CreateDirectionalLightMenuPanelImpl(DirectionalLight* directional_light);
	static void CreateFiltersMenuPanelImpl(Shader& shader, float time);
	static void CreateBackgroundMenuPanelImpl(glm::vec3& color);
	static void CreateModelLoaderPanelImpl(Model* model);
	static void CreateViewportImpl(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer);
	static void GeneratePanelsImpl(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer, Model* model, glm::vec3& background_color, Shader& shader, float time,
		DirectionalLight* directional_light, PointLight* point_light);
	static bool ViewportSelectedImpl();
	static bool ViewportHoveredImpl();
	static bool ShowSkyboxImpl();

	static void ToggleSkybox();
};