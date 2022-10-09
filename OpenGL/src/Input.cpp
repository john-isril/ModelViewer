#include "Input.h"

namespace Input
{
	bool IsKeyPressed(GLFWwindow* window, Key::KeyCode key)
	{
		auto state = glfwGetKey(window, key);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool IsKeyReleased(GLFWwindow* window, Key::KeyCode key)
	{
		return glfwGetKey(window, key) != GLFW_PRESS;
	}

	bool IsMouseButtonPressed(GLFWwindow* window, Mouse::MouseCode button)
	{
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}

	bool IsMouseButtonReleased(GLFWwindow* window, Mouse::MouseCode button)
	{
		return glfwGetMouseButton(window, button) != GLFW_PRESS;
	}

	std::pair<float, float> GetMouseCoordinates(GLFWwindow* window)
	{
		double xpos{}, ypos{};
		glfwGetCursorPos(window, &xpos, &ypos);

		return std::pair<float, float>(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}