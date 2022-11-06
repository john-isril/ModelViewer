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
#include "PointLight.h"
#include "DirectionalLight.h"
#include "Window.h"

constexpr char GLSL_version[]{ "#version 460" };

static float last_frame{ 0.0f };
static float delta_time{ 0.0f };

static void processInput(GLFWwindow* window);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void UpdateWindowDimensionDependencies(glm::mat4& projection_matrix, Window* window, Camera* camera = nullptr, FrameBuffer* fame_buffer = nullptr);

static Camera camera{ Transform{glm::vec3 {0.0f}, Rotation{0.0f, -90.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 3.0f}} };

int main()
{
    Window window{ "3D Model Viewer" };
    window.SetScrollCallback(scroll_callback);

    Editor::Init(window.GetGLFWwindow(), GLSL_version);

    Renderer renderer{};

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
    const char* cubemap_file_paths[]{
        "Assets/skybox/right.jpg",
        "Assets/skybox/left.jpg",
        "Assets/skybox/top.jpg",
        "Assets/skybox/bottom.jpg",
        "Assets/skybox/front.jpg",
        "Assets/skybox/back.jpg"
    };

    Skybox skybox{ cubemap_file_paths };

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
    
    FrameBuffer postprocess_frame_buffer{ window.GetScreenWidth(), window.GetScreenHeight()};

    skybox_shader.Bind();
    skybox_shader.SetUniform1i("skybox", 0);

    glm::vec3 background_color{ 1.0f, 1.0f, 1.0f };

    while (!window.Closed())
    {
        float current_frame{ static_cast<float>(glfwGetTime()) };

        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        processInput(window.GetGLFWwindow());

        postprocess_frame_buffer.Bind();
        renderer.Clear(background_color, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

         UpdateWindowDimensionDependencies(projection, &window, &camera, &postprocess_frame_buffer);
        
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

        postprocess_frame_buffer.Unbind();
        postprocessing_shader.Bind();
        postprocess_frame_buffer.BindTextureColorBuffer();
        
        renderer.DrawArrays(screen_quad_VAO, postprocessing_shader, NUM_OF_SCREEN_QUAD_VERTICES);

        Editor::BeginRender();
        Editor::CreateViewWindow("Viewport", projection, &window, &camera, &postprocess_frame_buffer);
        Editor::CreateTransformMenu("3D Model", &(model_3D.GetTransform()), TRANSFORM_TRANSLATION | TRANSFORM_ROTATION | TRANSFORM_SCALE);
        Editor::CreatePointLightMenu("Point Light", &point_light);
        Editor::CreateFiltersMenu("Filters", postprocessing_shader, glfwGetTime());
        Editor::CreateTextInput("3D Model File Path", &model_3D);
        Editor::CreateBackgroundMenu("Background", background_color);
        Editor::CreateDirectionalLightMenu("Directional Light", &directional_light);
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

static void UpdateWindowDimensionDependencies(glm::mat4 &projection_matrix, Window *window, Camera *camera, FrameBuffer *fame_buffer)
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
        }
    }
}