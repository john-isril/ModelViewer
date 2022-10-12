#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "Shader.h"

class Editor
{
public:

public:
	Editor();
	~Editor();
	Editor(const Editor&) = delete;
	Editor& operator=(const Editor&) = delete;
	Editor(Editor&&) = delete;
	Editor& operator=(Editor&&) = delete;

	static void Init(GLFWwindow* window, const char* GLSL_version);
	static void BeginRender();
	static void EndRender();
	static void Shutdown();

	static void CreateTransformMenu(const char* title, Transform &transform);
	static void CreateLightMenu(const char* title, float& brightness, glm::vec3& color, bool &is_on);
	static void CreateFiltersMenu(const char* title, Shader &shader, int &filter_type, float &vignette_intensity, float &blur_intensity, float time);
	static void CreateBackgroundMenu(const char* title, glm::vec3& color, bool& show_skybox);
	static bool MouseIsOnEditor();

private:
	static Editor m_instance;

private:
};