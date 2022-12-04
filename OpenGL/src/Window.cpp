#include "Window.h"
#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);

Window* callback_window_ref {nullptr};

Window::Window(const char* title, uint32_t width, uint32_t height) :
	m_title{title}, m_width{width}, m_height{height}, m_scroll_callback{nullptr}
{
	m_width_mid = static_cast<float>(m_width) / 2.0f;
	m_height_mid = static_cast<float>(m_height) / 2.0f;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }

    callback_window_ref = this;

    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(m_window, frame_buffer_size_callback);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::Update()
{
    m_width_prev = m_width;
    m_height_prev = m_height;
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool Window::Closed() const
{
    return (glfwWindowShouldClose(m_window));
}

void Window::Resize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    m_width_mid = static_cast<float>(m_width) / 2.0f;
    m_height_mid = static_cast<float>(m_height) / 2.0f;
}

void Window::SetScrollCallback(void (*scroll_callback)(GLFWwindow*, double, double))
{
    m_scroll_callback = scroll_callback;
    glfwSetScrollCallback(m_window, m_scroll_callback);
}

GLFWwindow* Window::GetGLFWwindow()
{
    return m_window;
}

uint32_t Window::GetScreenWidth() const
{
    return m_width;
}

uint32_t Window::GetScreenHeight() const
{
    return m_height;
}

uint32_t Window::GetScreenWidthMid() const
{
    return m_width_mid;
}

uint32_t Window::GetScreenHeightMid() const
{
    return m_height_mid;
}

bool Window::Resized() const
{
    if ((m_width != m_width_prev) || (m_height != m_height_prev))
    {
        return true;
        std::cout << "resized\n";
    }
    return false;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    callback_window_ref->Resize(width, height);
    glViewport(0, 0, width, height);
}