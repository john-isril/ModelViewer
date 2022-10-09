#pragma once
#include "KeyCodes.h"
#include "MouseCodes.h"
#include <GLFW/glfw3.h>
#include <utility>

namespace Input
{
	bool IsKeyPressed(GLFWwindow *window, Key::KeyCode key);
	bool IsKeyReleased(GLFWwindow* window, Key::KeyCode key);

	bool IsMouseButtonPressed(GLFWwindow* window, Mouse::MouseCode button);
	bool IsMouseButtonReleased(GLFWwindow* window, Mouse::MouseCode button);
	std::pair<float, float> GetMouseCoordinates(GLFWwindow* window);
}