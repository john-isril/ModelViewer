#include <glad/glad.h> // glad must be included before GLFW since glad includes opengl headers that GLFW needs
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

    GLFWwindow* window{ glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Learning OpenGL", nullptr, nullptr) };

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

    stbi_set_flip_vertically_on_load(true);

    std::cout << glGetString(GL_VERSION) << std::endl;

    editor.Init(window, GLSL_version);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugUtils::OpenGLMessageCallback, nullptr);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetScrollCallback(window, scroll_callback);

    Shader lighting_shader{ "Resources/lighting.shader" };
    Shader light_source_shader{ "Resources/light_source.shader" };
    Shader model_shader{ "Resources/model_loader.shader" };

    float vertices[]{
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

    unsigned int indices[]{
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

    VertexBuffer vb{ vertices, sizeof(vertices) };
    IndexBuffer ib{ indices, 36 };
    //Texture diffuse_map("Assets/container2.png", true, Texture::Type::None);
    //Texture specular_map("Assets/container2_specular.png", true, Texture::Type::None);
    //Texture emission_map("Assets/matrix.jpg", true, Texture::Type::None);
    VertexBufferLayout layout{};
    layout.AddAttribute<float>(3);
    layout.AddAttribute<float>(2);
    layout.AddAttribute<float>(3);

    VertexArray point_light_va{};
    point_light_va.AddBufferLayout(vb, layout);

    Renderer renderer{};

    glm::mat4 view{ 1.0f };
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100.0f);

    // object
    // glm::vec3 object_color{ 1.0f, 0.5f, 0.31f };  dont need this rn since were giving it textures

    ///////////////////////////////////////////////////////////////////////////// LIGHTS ////////////////////////////////////////////////////////////////////////////////////////////////////////

    glm::vec3 light_specular{ 1.0f }; // a lights specular component should have full intensity

    //////////////////////////////////////////////////// DIRECTIONAL LIGHT ////////////////////////////////////////////////////////////////////

    float directional_light_brightness{ 1.0f };
    glm::vec3 directional_light_color{ 1.0f, 1.0f, 1.0f };
    glm::vec3 directional_light_direction{ -0.2f, -1.0f, -0.3f };
    glm::vec3 directional_light_diffuse_color = directional_light_color * glm::vec3(0.5f);
    glm::vec3 directional_light_ambient_color = directional_light_color * glm::vec3(0.2f);


    //////////////////////////////////////////////////// POINT LIGHT ////////////////////////////////////////////////////////////////////

    float point_light_brightness{ 5.0f };
    glm::vec3 point_light_color{ 1.0f, 0.0f, 0.0f };
    //glm::vec3 point_light_position{ 0.2f, 1.0f, 0.3f };
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
    glm::mat4 point_light_mvp{ projection * view * point_light_model };

    //////////////////////////////////////////////////// SPOT LIGHT ////////////////////////////////////////////////////////////////////

    float spot_light_brightness{ 5.0f };
    glm::vec3 spot_light_color{ 1.0f, 1.0f, 1.0f };
    glm::vec3 spot_light_diffuse_color = spot_light_color * glm::vec3(0.5f);
    glm::vec3 spot_light_ambient_color = spot_light_color * glm::vec3(0.2f);
    float spot_light_constant{ 1.0f };
    float spot_light_linear{ 0.09f };
    float spot_light_quadratic{ 0.032f };
    float spot_light_cutoff{ 12.5f };
    float outer_spot_light_cutoff{ 15.0f };
    bool spot_light_is_on{ true };

    //////////////////////////////////////////////////// OBJECT ////////////////////////////////////////////////////////////////////
    Model ourModel("Assets/backpack/backpack.obj");

    glm::mat4 model_model{ 1.0f };
    glm::mat4 model_mvp{};
    
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    lighting_shader.Bind();
    lighting_shader.SetUniform1i("material.diffuse", diffuse_map.GetSlot());
    lighting_shader.SetUniform1i("material.specular", specular_map.GetSlot());
    lighting_shader.SetUniform1i("material.emission", emission_map.GetSlot());
    lighting_shader.SetUniform1f("material.shininess", 64.0f);

    // DIRECTIONAL
    lighting_shader.SetUniform1f("directional_light.brightness", directional_light_brightness);
    lighting_shader.SetUniformVec3f("directional_light.direction", directional_light_direction);
    lighting_shader.SetUniformVec3f("directional_light.ambient", directional_light_ambient_color);
    lighting_shader.SetUniformVec3f("directional_light.diffuse", directional_light_diffuse_color);
    lighting_shader.SetUniformVec3f("directional_light.specular", light_specular);

    // POINT
    lighting_shader.SetUniform1f("point_light.brightness", point_light_brightness);
    lighting_shader.SetUniform1f("point_light.constant", point_light_constant);
    lighting_shader.SetUniform1f("point_light.linear_", point_light_linear);
    lighting_shader.SetUniform1f("point_light.quadratic", point_light_quadratic);
    lighting_shader.SetUniformVec3f("point_light.position", point_light_transform.GetTranslation());
    lighting_shader.SetUniformVec3f("point_light.ambient", point_light_ambient_color);
    lighting_shader.SetUniformVec3f("point_light.diffuse", point_light_diffuse_color);
    lighting_shader.SetUniformVec3f("point_light.specular", light_specular);

    // SPOT
    lighting_shader.SetUniform1f("spot_light.brightness", spot_light_brightness);
    lighting_shader.SetUniformVec3f("spot_light.position", camera.GetPosition());
    lighting_shader.SetUniformVec3f("spot_light.direction", camera.GetForward());
    lighting_shader.SetUniformVec3f("spot_light.ambient", spot_light_ambient_color);
    lighting_shader.SetUniformVec3f("spot_light.diffuse", spot_light_diffuse_color);
    lighting_shader.SetUniformVec3f("spot_light.specular", light_specular);
    lighting_shader.SetUniform1f("spot_light.constant", spot_light_constant);
    lighting_shader.SetUniform1f("spot_light.linear_", spot_light_linear);
    lighting_shader.SetUniform1f("spot_light.quadratic", spot_light_quadratic);
    lighting_shader.SetUniform1f("spot_light.cutoff", glm::cos(glm::radians(spot_light_cutoff)));
    lighting_shader.SetUniform1f("spot_light.outer_cutoff", glm::cos(glm::radians(outer_spot_light_cutoff)));

    */
    // this normal is used to adjust the normals if the model changes due to something like scaling.
    glm::mat3 normal_matrix{};

   // diffuse_map.Bind();
    //specular_map.Bind();
    //emission_map.Bind();
    while (!glfwWindowShouldClose(window))
    {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        processInput(window);
        renderer.Clear(1.0f, 1.0f, 1.0f, 0.0f);

        view = camera.GetViewMatrix();

        /*point_light_model = glm::mat4(1.0f);
        point_light_model = glm::translate(point_light_model, point_light_transform.GetTranslation());
        rotate(point_light_model, point_light_transform.GetRotation());
        point_light_model = glm::scale(point_light_model, point_light_transform.GetScale());
        point_light_mvp = projection * view * point_light_model;
        point_light_diffuse_color = point_light_color * glm::vec3(0.5f);
        point_light_ambient_color = point_light_color * glm::vec3(0.2f);

        spot_light_diffuse_color = spot_light_color * glm::vec3(0.5f);
        spot_light_ambient_color = spot_light_color * glm::vec3(0.2f);

        light_source_shader.Bind();
        //point_light_va.Bind();
        light_source_shader.SetUniformMat4f("mvp", point_light_mvp);
        light_source_shader.SetUniformVec3f("light_color", point_light_color);
        renderer.DrawElements(point_light_va, ib, light_source_shader);


        lighting_shader.Bind();

        if (point_light_is_on)
        {
            lighting_shader.SetUniform1f("point_light.brightness", point_light_brightness);
        }
        else
        {
            lighting_shader.SetUniform1f("point_light.brightness", 0.0f);
        }
        lighting_shader.SetUniformVec3f("point_light.diffuse", point_light_diffuse_color);
        lighting_shader.SetUniformVec3f("point_light.ambient", point_light_ambient_color);
        lighting_shader.SetUniformVec3f("point_light.position", point_light_transform.GetTranslation());

        lighting_shader.SetUniformVec3f("spot_light.position", camera.GetPosition());
        if (spot_light_is_on)
        {
            lighting_shader.SetUniform1f("spot_light.brightness", spot_light_brightness);
        }
        else
        {
            lighting_shader.SetUniform1f("spot_light.brightness", 0.0f);
        }
        lighting_shader.SetUniformVec3f("spot_light.direction", camera.GetForward());
        lighting_shader.SetUniformVec3f("spot_light.diffuse", spot_light_diffuse_color);
        lighting_shader.SetUniformVec3f("spot_light.ambient", spot_light_ambient_color);
        */
        model_shader.Bind();
        model_model = glm::mat4(1.0f);
        model_model = glm::translate(model_model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model_model = glm::scale(model_model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        model_mvp = projection * view * model_model;
        model_shader.SetUniformMat4f("mvp", model_mvp);
        renderer.DrawModel(ourModel, model_shader);
        //ourModel.Draw(model_shader);
        /*
        editor.BeginRender();
        editor.CreateTransformMenu("Light Cube", point_light_transform);
        editor.CreateLightMenu("Point Light", point_light_brightness, point_light_color, point_light_is_on);
        editor.CreateLightMenu("Spot Light", spot_light_brightness, spot_light_color, spot_light_is_on);

        editor.EndRender();
        */
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