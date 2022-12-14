#include "Editor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include "Model.h"
#include "Input.h"
#include <iostream>

char model_input_buffer[160]{};

Editor Editor::m_instance;

void Editor::Init(GLFWwindow* window, const char* GLSL_version)
{
    Get().InitImpl(window, GLSL_version);
}

void Editor::BeginRender()
{
    Get().BeginRenderImpl();
}

void Editor::EndRender()
{
    Get().EndRenderImpl();
}

void Editor::Shutdown()
{
    Get().Shutdown();
}

void Editor::CreateTransformMenuPanel(const char* title, Transform* transform, uint8_t transform_flags)
{
    Get().CreateTransformMenuPanelImpl(title, transform, transform_flags);
}

void Editor::CreatePointLightMenuPanel(PointLight* point_light)
{
    Get().CreatePointLightMenuPanelImpl(point_light);
}

void Editor::CreateDirectionalLightMenuPanel(DirectionalLight* directional_light)
{
    Get().CreateDirectionalLightMenuPanelImpl(directional_light);
}

void Editor::CreateFiltersMenuPanel(Shader& shader, float time)
{
    Get().CreateFiltersMenuPanelImpl(shader, time);
}

void Editor::CreateBackgroundMenuPanel(glm::vec3& color)
{
    Get().CreateBackgroundMenuPanelImpl(color);
}

void Editor::CreateModelLoaderPanel(Model* model)
{
    Get().CreateModelLoaderPanelImpl(model);
}

void Editor::CreateViewport(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer)
{
    Get().CreateViewportImpl(projection_matrix, window, camera, frame_buffer);
}

void Editor::GeneratePanels(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer, Model* model, glm::vec3& background_color, Shader& shader, float time,
    DirectionalLight* directional_light, PointLight* point_light)
{
    Get().GeneratePanelsImpl(projection_matrix, window, camera, frame_buffer, model, background_color, shader, time, directional_light, point_light);
}

bool Editor::ViewportSelected()
{
    return Get().ViewportSelectedImpl();
}

bool Editor::ViewportHovered()
{
    return Get().ViewportHoveredImpl();
}

bool Editor::ShowSkybox()
{
    return Get().ShowSkyboxImpl();
}

Editor::~Editor()
{
}

Editor::Editor() :
    m_filter_type{0}, m_vignette_intensity{1.0f}, m_blur_intensity{0.07f}, m_show_skybox{true}, viewport_selected{false}, viewport_hovered{false}
{
}

Editor& Editor::Get()
{
    return m_instance;
}

void Editor::InitImpl(GLFWwindow* window, const char* GLSL_version)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
}

void Editor::BeginRenderImpl()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //bool show_demo_window = false;
    //ImGui::ShowDemoWindow(&show_demo_window);
}

void Editor::EndRenderImpl()
{
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Editor::ShutdownImpl()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Editor::CreateTransformMenuPanelImpl(const char* title, Transform *transform, uint8_t transform_flags)
{
    if (!transform)
    {
        std::cerr << "ERROR: Null point transform!\n";
    }
    
    if (Get().viewport_selected) ImGui::BeginDisabled();

    ImGui::Begin(title);
    ImGui::Text("Transform");

    if (transform_flags & TRANSFORM_TRANSLATION)
    {
        glm::vec3 translation{ transform->GetTranslation() };
        ImGui::DragFloat3("Position", &(translation.x), 0.01f);
        transform->SetTranslation(translation);
    }

    if (transform_flags & TRANSFORM_ROTATION)
    {
        Rotation rotation{ transform->GetRotation() };
        ImGui::DragFloat3("Rotation", &(rotation.pitch), 0.1f);
        transform->SetRotation(rotation);
    }

    if (transform_flags & TRANSFORM_SCALE)
    {
        glm::vec3 scale{ transform->GetScale() };
        if (transform_flags & TRANSFORM_SCALE_UNIFORM)
        {
            ImGui::DragFloat("Scale", &(scale.x), 0.01f, 0.0f, 50.0f);
            transform->SetScaleUniform(scale.x);
        }
        else
        {
            ImGui::DragFloat3("Scale", &(scale.x), 0.01f, 0.0f, 50.0f);
            transform->SetScale(scale);
        }
    }

    transform->UpdateModelMatrix();
    ImGui::End();

    if (Get().viewport_selected) ImGui::EndDisabled();
}

void Editor::CreatePointLightMenuPanelImpl(PointLight *point_light)
{
    if (!point_light)
    {
        std::cerr << "ERROR: Null point light!\n";
    }
    
    if (Get().viewport_selected) ImGui::BeginDisabled();

    ImGui::Begin(point_light->GetName().c_str());

    if (ImGui::Button("On/Off"))
    {
        point_light->Toggle();
    }
    if (ImGui::Button("Visible/Hidden"))
    {
        point_light->GetIsHidden() = 1 - point_light->GetIsHidden();
    }
    
    ImGui::DragFloat("Brightness", &(point_light->GetBrightness()), 0.1f, 0.0f, 10.0f);
    ImGui::ColorEdit3("Color", (float*)(&(point_light->GetColor())));
    CreateTransformMenuPanelImpl(point_light->GetName().c_str(), &(point_light->GetModel().GetTransform()), TRANSFORM_TRANSLATION | TRANSFORM_ROTATION | TRANSFORM_SCALE | TRANSFORM_SCALE_UNIFORM);
    point_light->UpdateColors();
    ImGui::End();

    if (Get().viewport_selected) ImGui::EndDisabled();
}

void Editor::CreateDirectionalLightMenuPanelImpl(DirectionalLight* directional_light)
{
    if (Get().viewport_selected) ImGui::BeginDisabled();

    ImGui::Begin(directional_light->GetName().c_str());
    ImGui::CaptureMouseFromApp(!(Get().viewport_selected));
    if (ImGui::Button("On/Off"))
    {
        directional_light->Toggle();
    }

    ImGui::DragFloat3("Direction", &(directional_light->GetDirection().x), 0.1f, -100.0f, 100.0f);
    ImGui::End();

    if (Get().viewport_selected) ImGui::EndDisabled();
}

void Editor::CreateFiltersMenuPanelImpl(Shader &shader, float time)
{
    if (Get().viewport_selected) ImGui::BeginDisabled();

    ImGui::Begin("Filters");
    ImGui::RadioButton("Normal", &Get().m_filter_type, 0);
    ImGui::RadioButton("Inverted", &Get().m_filter_type, 1);
    ImGui::RadioButton("Greyscale", &Get().m_filter_type, 2);
    ImGui::RadioButton("Vignette", &Get().m_filter_type, 3);
    ImGui::DragFloat("Vignette Intensity", &Get().m_vignette_intensity, 0.1f, 0.0f, 2.0f);
    ImGui::RadioButton("Blur Sweep", &Get().m_filter_type, 4);
    ImGui::DragFloat("Blur Intensity", &Get().m_blur_intensity, 0.001f, 0.0f, 0.07f);
    ImGui::End();

    if (Get().viewport_selected) ImGui::EndDisabled();

    shader.SetUniform1i("filter_type", Get().m_filter_type);
    shader.SetUniform1f("vignette_intensity", Get().m_vignette_intensity);
    shader.SetUniform1f("time", time);
    shader.SetUniform1f("blur_intensity", Get().m_blur_intensity);
}

void Editor::CreateBackgroundMenuPanelImpl(glm::vec3& color)
{
    if (Get().viewport_selected) ImGui::BeginDisabled();

    ImGui::Begin("Background");
    ImGui::CaptureMouseFromApp(!(Get().viewport_selected));
    if (ImGui::Button("Show Skybox"))
    {
        Get().ToggleSkybox();
    }
    ImGui::ColorEdit3("Color", (float*)&(color));
    ImGui::End();

    if (Get().viewport_selected) ImGui::EndDisabled();
}

void Editor::CreateModelLoaderPanelImpl(Model *model)
{
    if (!model)
    {
        std::cerr << "ERROR: Null model!\n";
    }
    
    if (Get().viewport_selected) ImGui::BeginDisabled();

    ImGui::Begin("3D Model Loader");
    ImGui::InputText("3D Model File Path", model_input_buffer, 160);

    if (ImGui::Button("Load model"))
    {
        model->LoadNewModel(std::string(model_input_buffer));
    }
    
    ImGui::End();

    if (Get().viewport_selected) ImGui::EndDisabled();
}

void Editor::CreateViewportImpl(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer)
{
    static ImVec2 image_size{};
    static glm::vec2 current_window_size{};
    static glm::vec2 last_window_size{};

    constexpr ImVec2 uv_x{ 0, 1 };
    constexpr ImVec2 uv_y{ 1, 0 };

    ImGui::Begin("Viewport");
    ImVec2 panel_size{ ImGui::GetWindowSize() };
    panel_size.y -= 35;

    if (camera && ((image_size.x != panel_size.x) || (image_size.y != panel_size.y)))
    {
        image_size = panel_size;
        projection_matrix = glm::perspective(glm::radians(camera->GetFieldOfView()), static_cast<float>(image_size.x) / static_cast<float>(image_size.y), camera->GetNearPlaneDistance(), camera->GetFarPlaneDistance());
    }
    else if (!camera)
    {
        std::cerr << "ERROR: Null camera!\n";
    }

    ImGui::Image((void*)(intptr_t)frame_buffer->GetTextureColorBufferID(), image_size, uv_x, uv_y);
    
    if (window)
    {
        bool selecting{ Input::IsMouseButtonPressed(window->GetGLFWwindow(), Mouse::LeftClick) || Input::IsMouseButtonPressed(window->GetGLFWwindow(), Mouse::RightClick) };
        Get().viewport_hovered = ImGui::IsWindowHovered();

        if (Get().viewport_hovered && selecting && Get().viewport_selected == false)
        {
            Get().viewport_selected = true;
        }
        else if (!selecting)
        {
            Get().viewport_selected = false;
        }
    }
    
    ImGui::End();
}

void Editor::GeneratePanelsImpl(glm::mat4& projection_matrix, Window* window, Camera* camera, FrameBuffer* frame_buffer, Model* model, glm::vec3& background_color, Shader &shader, float time, DirectionalLight* directional_light, PointLight* point_light)
{
    Get().CreateViewportImpl(projection_matrix, window, camera, frame_buffer);
    Get().CreateTransformMenuPanelImpl(model->GetName().c_str(), &(model->GetTransform()), TRANSFORM_TRANSLATION | TRANSFORM_ROTATION | TRANSFORM_SCALE);
    Get().CreatePointLightMenuPanelImpl(point_light);
    Get().CreateFiltersMenuPanelImpl(shader, time);
    Get().CreateModelLoaderPanelImpl(model);
    Get().CreateBackgroundMenuPanelImpl(background_color);
    Get().CreateDirectionalLightMenuPanelImpl(directional_light);
}

bool Editor::ViewportSelectedImpl()
{
    return Get().viewport_selected;
}

bool Editor::ViewportHoveredImpl()
{
    return Get().viewport_hovered;
}

bool Editor::ShowSkyboxImpl()
{
    return Get().m_show_skybox;
}

void Editor::ToggleSkybox()
{
    Get().m_show_skybox = 1 - (Get().m_show_skybox);
}