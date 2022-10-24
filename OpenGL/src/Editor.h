#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "Shader.h"
#include "PointLight.h"

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
	static void CreatePointLightMenu(const char* title, PointLight *point_light);
	static void CreateFiltersMenu(const char* title, Shader &shader, int &filter_type, float &vignette_intensity, float &blur_intensity, float time);
	static void CreateBackgroundMenu(const char* title, glm::vec3& color, bool& show_skybox);
	static void CreateTextInput(const char* title, class Model* model);
	static bool MouseIsOnEditor();

private:
	static Editor m_instance;

private:
};
/*
#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "Shader.h"
#include "Model.h"

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

	static void CreateTransformMenu(const char* title, Transform& transform);
	static void CreateLightMenu(const char* title, float& brightness, glm::vec3& color, bool& is_on);
	static void CreateFiltersMenu(const char* title, Shader& shader, int& filter_type, float& vignette_intensity, float& blur_intensity, float time);
	static void CreateBackgroundMenu(const char* title, glm::vec3& color, bool& show_skybox);
	static void CreateTextInput(const char* title, Model* model);
	static bool MouseIsOnEditor();

private:
	static Editor m_instance;
	static char model_input_buffer[160];

private:
};
*/