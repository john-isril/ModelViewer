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
#include "DebugUtils.h"
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

static bool InitOpenGL();
static void processInput(GLFWwindow* window);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static Camera camera{ Transform{glm::vec3 {0.0f}, Rotation{0.0f, -90.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 3.0f}} };
static Editor editor{};

int main()
{
    Window window{ "3D Model Viewer" };
    window.SetScrollCallback(scroll_callback);

    editor.Init(window.GetGLFWwindow(), GLSL_version);

    if (!InitOpenGL()) return -1;

    Shader light_source_shader{ "Resources/light_source.shader" };
    Shader model_shader{ "Resources/model.shader" };
    Shader postprocessing_shader{ "Resources/postprocessing.shader" };
    Shader skybox_shader{ "Resources/skybox.shader" };

    ////////////////////////////////////////////////////////////////////////////////////////// LIGHTS ///////////////////////////////////////////////////////////////////////////////////////

    PointLight point_light{ "Assets/sphere/sphere.obj" };
    DirectionalLight directional_light{};

    ////////////////////////////////////////////////////////// POST PROCESSING SCREEN QUAD /////////////////////////////////////////////////
    constexpr uint8_t NUM_OF_SCREEN_QUAD_VERTICES{ 6 };

    float screen_quad_vertices[] = {
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
    Renderer renderer{};

    glm::mat4 view{ 1.0f };
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.GetScreenWidth()) / static_cast<float>(window.GetScreenHeight()), 0.1f, 100.0f);

    model_shader.Bind();
    
    model_shader.SetUniform1f("directional_light.brightness", directional_light.GetBrightness());
    model_shader.SetUniformVec3f("directional_light.direction", directional_light.GetDirection());
    model_shader.SetUniformVec3f("directional_light.ambient", directional_light.GetAmbient());
    model_shader.SetUniformVec3f("directional_light.diffuse", directional_light.GetDiffuse());
    model_shader.SetUniformVec3f("directional_light.specular", directional_light.GetSpecular());

    
    model_shader.SetUniform1f("point_light.brightness", point_light.GetBrightness());
    model_shader.SetUniform1f("point_light.constant", point_light.GetConstant());
    model_shader.SetUniform1f("point_light.linear_", point_light.GetLinear());
    model_shader.SetUniform1f("point_light.quadratic", point_light.GetQuadratic());
    model_shader.SetUniformVec3f("point_light.position", point_light.GetModel().GetTransform().GetTranslation());
    model_shader.SetUniformVec3f("point_light.ambient", point_light.GetAmbient());
    model_shader.SetUniformVec3f("point_light.diffuse", point_light.GetDiffuse());
    model_shader.SetUniformVec3f("point_light.specular", point_light.GetSpecular());
    model_shader.SetUniform1f("material.shininess", 16.0f);

    postprocessing_shader.Bind();
    postprocessing_shader.SetUniform1i("screen_quad_texture", 0);
    
    FrameBuffer postprocess_frame_buffer{ window.GetScreenWidth(), window.GetScreenHeight()};

    skybox_shader.Bind();
    skybox_shader.SetUniform1i("skybox", 0);

    // Would like to use an enum for this, however, GLSL has no support for enums so I'll be using an int instead.
    // DearIMGui also expects an int when using the radio button feature.
    int filter_type{0};
    float vignette_intensity{1.0f};
    float blur_intensity{ 0.07f };
    bool show_skybox{ true };
    glm::vec3 background_color{ 1.0f, 1.0f, 1.0f };

    while (!window.Closed())
    {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        processInput(window.GetGLFWwindow());

        postprocess_frame_buffer.Bind();
        renderer.Clear(background_color.r, background_color.g, background_color.b, 1.0f);

        view = camera.GetViewMatrix();

        point_light.GetModel().GetTransform().UpdateMVP(view, projection);

        light_source_shader.Bind();
        light_source_shader.SetUniformMat4f("mvp", point_light.GetModel().GetTransform().GetMVPMatrix());
        light_source_shader.SetUniformVec3f("light_color", point_light.GetColor());
        
        if (!point_light.GetIsHidden())
        {
            renderer.DrawModel(point_light.GetModel(), light_source_shader);
        }
        
        light_source_shader.Bind();
        light_source_shader.SetUniform1f("light_brightness", point_light.GetBrightnessOffOn());
        model_shader.Bind();
        model_shader.SetUniform1f("point_light.brightness", point_light.GetBrightnessOffOn());

        model_shader.SetUniformVec3f("point_light.diffuse", point_light.GetDiffuse());
        model_shader.SetUniformVec3f("point_light.ambient", point_light.GetAmbient());
        model_shader.SetUniformVec3f("point_light.position", point_light.GetModel().GetTransform().GetTranslation());

        model_3D.GetTransform().UpdateMVP(view, projection);
        model_shader.SetUniformMat4f("model", model_3D.GetTransform().GetModelMatrix());
        model_shader.SetUniformMat4f("mvp", model_3D.GetTransform().GetMVPMatrix());
        model_shader.SetUniformVec3f("view_position", camera.GetPosition());

        renderer.DrawModel(model_3D, model_shader);

        if (show_skybox)
        {
            renderer.DrawSkybox(skybox, camera.GetViewMatrix(), projection, skybox_shader);
        }

        postprocess_frame_buffer.Unbind();
        postprocessing_shader.Bind();
        postprocess_frame_buffer.BindTextureColorBuffer();
        
        renderer.DrawArrays(screen_quad_VAO, postprocessing_shader, NUM_OF_SCREEN_QUAD_VERTICES);

        editor.BeginRender();
        editor.CreateTransformMenu("3D Model", &(model_3D.GetTransform()), TRANSFORM_TRANSLATION | TRANSFORM_ROTATION | TRANSFORM_SCALE);
        editor.CreatePointLightMenu("Point Light", &point_light);
        editor.CreateFiltersMenu("Filters", postprocessing_shader, filter_type, vignette_intensity, blur_intensity, glfwGetTime());
        editor.CreateTextInput("3D Model File Path", &model_3D);
        editor.CreateBackgroundMenu("Background", background_color, show_skybox);
        editor.EndRender();
        glfwSwapBuffers(window.GetGLFWwindow()); 
        glfwPollEvents();
    }

    editor.Shutdown();

    return 0;
}

static bool InitOpenGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return false;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_MULTISAMPLE);
    glDebugMessageCallback(DebugUtils::OpenGLMessageCallback, nullptr);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

static void processInput(GLFWwindow* window)
{
    if (Input::IsKeyPressed(window, Key::Escape))
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    if (!editor.MouseIsOnEditor())
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
    if (!editor.MouseIsOnEditor())
    {
        float z_offset = static_cast<float>(yoffset) * 400.0f;
        camera.Walk(0.0f, z_offset, delta_time);
    }
}