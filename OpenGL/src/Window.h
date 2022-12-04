#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Window
{
public:
	Window(const char* title, uint32_t width = 1440, uint32_t height = 810);
	~Window();
	void Update();
	bool Closed() const;
	void Resize(uint32_t width, uint32_t height);

	void SetScrollCallback(void (*scroll_callback)(GLFWwindow*, double, double));

	GLFWwindow* GetGLFWwindow();
	uint32_t GetScreenWidth() const;
	uint32_t GetScreenHeight() const;
	uint32_t GetScreenWidthMid() const;
	uint32_t GetScreenHeightMid() const;
	bool Resized() const;

private:
	const char* m_title;
	uint32_t m_width_prev, m_height_prev;
	uint32_t m_width, m_height;
	uint32_t m_width_mid, m_height_mid;
	GLFWwindow* m_window;

	void (*m_scroll_callback)(GLFWwindow*, double, double);
};