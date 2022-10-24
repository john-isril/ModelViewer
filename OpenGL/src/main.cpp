#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
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
#include <stdio.h>
#include <string.h>

static constexpr unsigned int  SCREEN_WIDTH{ 16 * 90 };
static constexpr unsigned int SCREEN_HEIGHT{ 9 * 90 };
static constexpr float SCREEN_MID_X{ static_cast<float>(SCREEN_WIDTH) / 2.0f };
static constexpr float SCREEN_MID_Y{ static_cast<float>(SCREEN_HEIGHT) / 2.0f };
constexpr char GLSL_version[]{ "#version 460" };

static bool first_mouse{ true };
float last_x_pos{ SCREEN_MID_X };
float last_y_pos{ SCREEN_MID_Y };
static float last_frame{ 0.0f };
static float delta_time{ 0.0f };

static void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
static void processInput(GLFWwindow* window);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
static void rotate(glm::mat4& model, const Rotation& rotation);

static Camera camera{ Transform{glm::vec3 {0.0f}, Rotation{0.0f, -90.0f, 0.0f}, glm::vec3 {0.0f, 0.0f, 3.0f}} };
static Editor editor{};

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window{ glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "3D Model Viewer", nullptr, nullptr) };

    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    editor.Init(window, GLSL_version);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_MULTISAMPLE);
    glDebugMessageCallback(DebugUtils::OpenGLMessageCallback, nullptr);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetScrollCallback(window, scroll_callback);

    Shader light_source_shader{ "Resources/light_source.shader" };
    Shader model_shader{ "Resources/model.shader" };
    Shader postprocessing_shader{ "Resources/postprocessing.shader" };
    Shader skybox_shader{ "Resources/skybox.shader" };

    ////////////////////////////////////////////////////////////////////////////////////////// LIGHTBOX/POINT LIGHT ///////////////////////////////////////////////////////////////////////////////////////
    glm::vec3 light_specular{ 1.0f };

    float light_box_vertices[]
    {
        // back face
        //position             // texture_coords        // normal
        0.5f, 0.5f, 0.5f,          1.0f, 1.0f,          0.0f, 0.0f, 1.0f,                                       // top right
        0.5f, -0.5f, 0.5f,         1.0f, 0.0f,          0.0f, 0.0f, 1.0f,                       // bottom right
        -0.5f, -0.5f, 0.5f,        0.0f, 0.0f,          0.0f, 0.0f, 1.0f,                 // bottom left
        -0.5f, 0.5f, 0.5f,         0.0f, 1.0f,          0.0f, 0.0f, 1.0f,                 // top left

        // front face
        0.5f, 0.5f, -0.5f,         0.0f, 1.0f,          0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,        0.0f, 0.0f,          0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,       1.0f, 0.0f,          0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f,        1.0f, 1.0f,          0.0f, 0.0f, -1.0f,

        // top face
        0.5f, 0.5f, -0.5,          1.0f, 1.0f,          0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5,           1.0f, 0.0f,          0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5,          0.0f, 0.0f,          0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5,         0.0f, 1.0f,          0.0f, 1.0f, 0.0f,

        //bottom face
        0.5f, -0.5f, 0.5,          1.0f, 1.0f,          0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5,         1.0f, 0.0f,          0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5,        0.0f, 0.0f,          0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5,         0.0f, 1.0f,          0.0f, -1.0f, 0.0f,

        //right face
        0.5f, 0.5, -0.5,           1.0f, 1.0f,          1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,        1.0f, 0.0f,          1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,         0.0f, 0.0f,          1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,          0.0f, 1.0f,          1.0f, 0.0f, 0.0f,

        //left face
        -0.5f, 0.5, 0.5,           1.0f, 1.0f,          -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,        1.0f, 0.0f,          -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,       0.0f, 0.0f,          -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f,        0.0f, 1.0f,          -1.0f, 0.0f, 0.0f
    };

    unsigned int light_box_indices[]
    {
        0, 1, 3,
        1, 2, 3,

        4, 5, 7,
        5, 6, 7,

        8, 9, 11,
        9, 10, 11,

        12, 13, 15,
        13, 14, 15,

        16, 17, 19,
        17, 18, 19,

        20, 21, 23,
        21, 22, 23
    };

    VertexBuffer vb{ light_box_vertices, sizeof(light_box_vertices) };
    IndexBuffer ib{ light_box_indices, 36 };
    VertexBufferLayout layout{};
    layout.AddAttribute<float>(3);
    layout.AddAttribute<float>(2);
    layout.AddAttribute<float>(3);

    VertexArray point_light_va{};
    point_light_va.AddBufferLayout(vb, layout);

    float point_light_brightness{ 1.0f };
    glm::vec3 point_light_color{ 1.0f, 1.0f, 1.0f };
    glm::vec3 point_light_diffuse_color = point_light_color * glm::vec3(0.5f);
    glm::vec3 point_light_ambient_color = point_light_color * glm::vec3(0.2f);
    float point_light_constant{ 1.0f };
    float point_light_linear{ 0.09f };
    float point_light_quadratic{ 0.032f };
    bool point_light_is_on{ true };

    glm::mat4 point_light_model{ 1.0f };
    Transform point_light_transform{ glm::vec3{1.0f, 1.0f, 1.0f}, Rotation{}, glm::vec3{1.2f, 1.0f, 2.0f} };
    point_light_model = glm::translate(point_light_model, point_light_transform.GetTranslation());
    rotate(point_light_model, point_light_transform.GetRotation());
    point_light_model = glm::scale(point_light_model, point_light_transform.GetScale());
    glm::mat4 point_light_mvp{};

    //////////////////////////////////////////////////// DIRECTIONAL LIGHT ////////////////////////////////////////////////////////////////////

    float directional_light_brightness{ 1.0f };
    glm::vec3 directional_light_color{ 1.0f, 1.0f, 1.0f };
    glm::vec3 directional_light_direction{ -0.2f, -1.0f, -0.3f };
    glm::vec3 directional_light_diffuse_color = directional_light_color * glm::vec3(0.5f);
    glm::vec3 directional_light_ambient_color = directional_light_color * glm::vec3(0.2f);

    constexpr uint8_t NUM_OF_SCREEN_QUAD_VERTICES{ 6 };

    ////////////////////////////////////////////////////////// POST PROCESSING SCREEN QUAD /////////////////////////////////////////////////
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
    Model model_3D("Assets/sphere/backpack.obj");
    Renderer renderer{};

    glm::mat4 view{ 1.0f };
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);

    model_shader.Bind();
    
    model_shader.SetUniform1f("directional_light.brightness", directional_light_brightness);
    model_shader.SetUniformVec3f("directional_light.direction", directional_light_direction);
    model_shader.SetUniformVec3f("directional_light.ambient", directional_light_ambient_color);
    model_shader.SetUniformVec3f("directional_light.diffuse", directional_light_diffuse_color);
    model_shader.SetUniformVec3f("directional_light.specular", light_specular);

    
    model_shader.SetUniform1f("point_light.brightness", point_light_brightness);
    model_shader.SetUniform1f("point_light.constant", point_light_constant);
    model_shader.SetUniform1f("point_light.linear_", point_light_linear);
    model_shader.SetUniform1f("point_light.quadratic", point_light_quadratic);
    model_shader.SetUniformVec3f("point_light.position", point_light_transform.GetTranslation());
    model_shader.SetUniformVec3f("point_light.ambient", point_light_ambient_color);
    model_shader.SetUniformVec3f("point_light.diffuse", point_light_diffuse_color);
    model_shader.SetUniformVec3f("point_light.specular", light_specular);

    postprocessing_shader.Bind();
    postprocessing_shader.SetUniform1i("screen_quad_texture", 0);
    
    FrameBuffer postprocess_frame_buffer{ SCREEN_WIDTH, SCREEN_HEIGHT };

    skybox_shader.Bind();
    skybox_shader.SetUniform1i("skybox", 0);

    // Would like to use an enum for this, however, GLSL has no support for enums so I'll be using an int instead.
    // DearIMGui also expects an int when using the radio button feature.
    int filter_type{0};
    float vignette_intensity{1.0f};
    float blur_intensity{ 0.07f };
    bool show_skybox{ true };
    glm::vec3 background_color{ 1.0f, 1.0f, 1.0f };

    while (!glfwWindowShouldClose(window))
    {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        processInput(window);

        postprocess_frame_buffer.Bind();
        renderer.Clear(background_color.r, background_color.g, background_color.b, 1.0f);

        view = camera.GetViewMatrix();

        point_light_model = glm::mat4(1.0f);
        point_light_model = glm::translate(point_light_model, point_light_transform.GetTranslation());
        rotate(point_light_model, point_light_transform.GetRotation());
        point_light_model = glm::scale(point_light_model, point_light_transform.GetScale());
        point_light_mvp = projection * view * point_light_model;
        point_light_diffuse_color = point_light_color * glm::vec3(0.5f);
        point_light_ambient_color = point_light_color * glm::vec3(0.2f);

        light_source_shader.Bind();
        light_source_shader.SetUniformMat4f("mvp", point_light_mvp);
        light_source_shader.SetUniformVec3f("light_color", point_light_color);
        renderer.DrawElements(point_light_va, ib, light_source_shader);

        model_shader.Bind();

        if (point_light_is_on)
        {
            model_shader.SetUniform1f("point_light.brightness", point_light_brightness);
        }
        else
        {
            model_shader.SetUniform1f("point_light.brightness", 0.0f);
        }

        model_shader.SetUniformVec3f("point_light.diffuse", point_light_diffuse_color);
        model_shader.SetUniformVec3f("point_light.ambient", point_light_ambient_color);
        model_shader.SetUniformVec3f("point_light.position", point_light_transform.GetTranslation());
        model_shader.SetUniform1f("material.shininess", 16.0f);

        model_3D.GetTransform().UpdateModelMatrix();
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
        editor.CreateTransformMenu("3D Model", model_3D.GetTransform());
        editor.CreateTransformMenu("Light Cube", point_light_transform);
        editor.CreateLightMenu("Point Light", point_light_brightness, point_light_color, point_light_is_on);
        editor.CreateFiltersMenu("Filters", postprocessing_shader, filter_type, vignette_intensity, blur_intensity, glfwGetTime());
        editor.CreateTextInput("3D Model File Path", &model_3D);
        editor.CreateBackgroundMenu("Background", background_color, show_skybox);
        editor.EndRender();
        glfwSwapBuffers(window); 
        glfwPollEvents();
    }

    editor.Shutdown();
    glfwTerminate();

    return 0;
}

static void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

void rotate(glm::mat4& model, const Rotation& rotation)
{
    model = glm::rotate(model, glm::radians(rotation.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.roll), glm::vec3(0.0f, 0.0f, 1.0f));
}