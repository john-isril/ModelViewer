#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"
#include "Renderer.h"
#include "Camera.h"
#include "Editor.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "FilterType.h"
#include "Skybox.h"
#include "Lights/PointLight.h"
#include "Lights/DirectionalLight.h"
#include "Window.h"
#include "Debugging/Log.h"

constexpr char GLSL_version[]{ "#version 460" };

static float last_frame{ 0.0f };
static float delta_time{ 0.0f };

static void processInput(GLFWwindow* window);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static bool UpdateWindowDimensionDependencies(glm::mat4& projection_matrix, Window* window, Camera* camera = nullptr, FrameBuffer* fame_buffer = nullptr);

static Camera camera{ Transform{glm::vec3 {0.0f}, Rotation{0.0f, -90.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 3.0f}} };

int main()
{
    Log::Init();
    LOG_INFO("Logger initialized!\n");

    Window window{ "3D Model Viewer" };
    window.SetScrollCallback(scroll_callback);

    Renderer renderer{};
    Editor::Init(window.GetGLFWwindow(), GLSL_version);

    Shader light_sphere_shader{ "Resources/light_sphere.shader" };
    Shader outline_shader{ "Resources/outline.shader" };
    Shader model_shader{ "Resources/model.shader" };
    Shader postprocessing_shader{ "Resources/postprocessing.shader" };
    Shader skybox_shader{ "Resources/skybox.shader" };

    float field_of_view{ 45.0f };
    glm::mat4 projection{ glm::perspective(glm::radians(field_of_view), static_cast<float>(window.GetScreenWidth()) / static_cast<float>(window.GetScreenHeight()), 0.1f, 100.0f) };

    ////////////////////////////////////////////////////////////////////////////////////////// LIGHTS ///////////////////////////////////////////////////////////////////////////////////////

    PointLight point_light{ "Assets/sphere/sphere.obj" };
    DirectionalLight directional_light{};

    ////////////////////////////////////////////////////////// POST PROCESSING SCREEN QUAD /////////////////////////////////////////////////
    constexpr uint8_t NUM_OF_SCREEN_QUAD_VERTICES{ 6 };

    float screen_quad_vertices[] {
        // positions   // texture coordinates
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    VertexBuffer screen_quad_buffer{ screen_quad_vertices, sizeof(screen_quad_vertices) };
    VertexBufferLayout screen_quad_layout{};
    screen_quad_layout.AddAttribute<float>(2);
    screen_quad_layout.AddAttribute<float>(2);
    VertexArray screen_quad_VAO{};
    screen_quad_VAO.AddBufferLayout(screen_quad_buffer, screen_quad_layout);

    //////////////////////////////////////////////////////////////////////// SKYBOX ///////////////////////////////////////////////////////////////

    Skybox skybox{ "Assets/skybox/SanFrancisco4/posx.jpg",
        "Assets/skybox/SanFrancisco4/negx.jpg",
        "Assets/skybox/SanFrancisco4/posy.jpg",
        "Assets/skybox/SanFrancisco4/negy.jpg",
        "Assets/skybox/SanFrancisco4/posz.jpg",
        "Assets/skybox/SanFrancisco4/negz.jpg" };

    //////////////////////////////////////////////////////////////////////// 3D MODEL ///////////////////////////////////////////////////////////////
    Model model_3D("Assets/backpack/backpack.obj");

    model_shader.Bind();
    
    model_shader.SetUniform1f("directional_light.brightness", directional_light.GetBrightness());
    model_shader.SetUniformVec3f("directional_light.direction", directional_light.GetDirection());
    model_shader.SetUniformVec4f("directional_light.ambient", directional_light.GetAmbient());
    model_shader.SetUniformVec4f("directional_light.diffuse", directional_light.GetDiffuse());
    model_shader.SetUniformVec4f("directional_light.specular", directional_light.GetSpecular());

    model_shader.SetUniform1f("point_light.brightness", point_light.GetBrightness());
    model_shader.SetUniform1f("point_light.constant", point_light.GetConstant());
    model_shader.SetUniform1f("point_light.linear_", point_light.GetLinear());
    model_shader.SetUniform1f("point_light.quadratic", point_light.GetQuadratic());
    model_shader.SetUniformVec3f("point_light.position", point_light.GetModel().GetTransform().GetTranslation());
    model_shader.SetUniformVec4f("point_light.ambient", point_light.GetAmbient());
    model_shader.SetUniformVec4f("point_light.diffuse", point_light.GetDiffuse());
    model_shader.SetUniformVec4f("point_light.specular", point_light.GetSpecular());
    model_shader.SetUniform1f("material.shininess", 16.0f);

    postprocessing_shader.Bind();
    postprocessing_shader.SetUniform1i("screen_quad_texture", 0);

    FrameBuffer pre_processed_framebuffer{ window.GetScreenWidth(), window.GetScreenHeight()};
    FrameBuffer post_processed_framebuffer{ window.GetScreenWidth(), window.GetScreenHeight() };

    skybox_shader.Bind();
    skybox_shader.SetUniform1i("skybox", 0);

    glm::vec3 background_color{ 1.0f, 1.0f, 1.0f };

    while (!window.Closed())
    {
        float current_frame{ static_cast<float>(glfwGetTime()) };
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        processInput(window.GetGLFWwindow());

        pre_processed_framebuffer.Bind();
        renderer.Clear(background_color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        bool window_resized{ UpdateWindowDimensionDependencies(projection, &window, &camera, &pre_processed_framebuffer) };

        point_light.GetModel().GetTransform().UpdateMVP(camera.GetViewMatrix(), projection);

        if (!point_light.GetIsHidden())
        {
            renderer.EnableOutlining();

            light_sphere_shader.Bind();
            light_sphere_shader.SetUniformMat4f("mvp", point_light.GetModel().GetTransform().GetMVPMatrix());
            light_sphere_shader.SetUniformVec4f("light_color", point_light.GetColor());
            light_sphere_shader.SetUniform1f("light_brightness", point_light.GetBrightnessOffOn());
            renderer.DrawModel(point_light.GetModel(), light_sphere_shader);

            renderer.DrawModelOutline(point_light.GetModel(), camera.GetViewMatrix(), projection, outline_shader, 0.005f, glm::vec3{0.0f});
            renderer.DisableOutlining();
        }

        model_shader.Bind();
        model_shader.SetUniform1f("point_light.brightness", point_light.GetBrightnessOffOn());

        model_shader.SetUniformVec4f("point_light.diffuse", point_light.GetDiffuse());
        model_shader.SetUniformVec4f("point_light.ambient", point_light.GetAmbient());
        model_shader.SetUniformVec3f("point_light.position", point_light.GetModel().GetTransform().GetTranslation());
        model_shader.SetUniform1f("directional_light.brightness", directional_light.GetBrightnessOffOn());
        model_shader.SetUniformVec3f("directional_light.direction", directional_light.GetDirection());

        model_3D.GetTransform().UpdateMVP(camera.GetViewMatrix(), projection);
        model_shader.SetUniformMat4f("model", model_3D.GetTransform().GetModelMatrix());
        model_shader.SetUniformMat4f("mvp", model_3D.GetTransform().GetMVPMatrix());
        model_shader.SetUniformVec3f("view_position", camera.GetPosition());

        renderer.DrawModel(model_3D, model_shader);
        
        if (Editor::ShowSkybox())
        {
            renderer.DrawSkybox(skybox, camera.GetViewMatrix(), projection, skybox_shader);
        }

        pre_processed_framebuffer.Unbind();
        pre_processed_framebuffer.BindTextureColorBuffer();
        post_processed_framebuffer.Bind();
        renderer.Clear(background_color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        if (window_resized)
        {
            post_processed_framebuffer.Resize(window.GetScreenWidth(), window.GetScreenHeight());
        }

        postprocessing_shader.Bind();
        renderer.DrawArrays(screen_quad_VAO, postprocessing_shader, NUM_OF_SCREEN_QUAD_VERTICES);
        post_processed_framebuffer.Unbind();
        post_processed_framebuffer.BindTextureColorBuffer();
        Editor::BeginRender();
        Editor::GeneratePanels(projection, &window, &camera, &post_processed_framebuffer, &model_3D, background_color, postprocessing_shader, current_frame, &directional_light, &point_light);
        Editor::EndRender();

        window.Update();
    }

    Editor::Shutdown();

    return 0;
}

static void processInput(GLFWwindow* window)
{ 
    if (Input::IsKeyPressed(window, Key::Escape))
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    if (Editor::ViewportSelected())
    {
        Camera::State last_camera_state{ camera.GetState() };
        camera.ProcessMouseButtonPress(Input::IsMouseButtonPressed(window, Mouse::LeftClick), Input::IsMouseButtonPressed(window, Mouse::RightClick));
        Camera::State camera_state{ camera.GetState() };

        if (camera_state != last_camera_state)
        {
            if (camera_state == Camera::State::Rotating || camera_state == Camera::State::Walking || camera_state == Camera::State::Panning)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        auto [mouse_x_pos, mouse_y_pos] {Input::GetMouseCoordinates(window)};
        camera.ProcessMouseMovement(mouse_x_pos, mouse_y_pos, delta_time);
    }

    if (Input::IsKeyPressed(window, Key::Space))
    {
        camera.LevelOrientation();
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (Editor::ViewportHovered())
    {
        float z_offset{ static_cast<float>(yoffset) * 400.0f };
        camera.Walk(0.0f, z_offset, delta_time);
    }
}

static bool UpdateWindowDimensionDependencies(glm::mat4 &projection_matrix, Window *window, Camera *camera, FrameBuffer *fame_buffer)
{
    static uint32_t current_window_width{};
    static uint32_t current_window_height{};

    if (window && ((current_window_width != window->GetScreenWidth()) || (current_window_height != window->GetScreenHeight())))
    {
        current_window_width = window->GetScreenWidth();
        current_window_height = window->GetScreenHeight();

        if (fame_buffer)
        {
            fame_buffer->Resize(current_window_width, current_window_height);
            projection_matrix = glm::perspective(glm::radians(camera->GetFieldOfView()), static_cast<float>(current_window_width) / static_cast<float>(current_window_height), camera->GetNearPlaneDistance(), camera->GetFarPlaneDistance());
            return true;
        }
    }

    return false;
}