#pragma once
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"

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
	static bool MouseIsOnEditor();

private:
	static Editor m_instance;

private:
};