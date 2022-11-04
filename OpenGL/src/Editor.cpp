#include "Editor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include "Model.h"

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

void Editor::CreateTransformMenu(const char* title, Transform* transform, uint8_t transform_flags)
{
    Get().CreateTransformMenuImpl(title, transform, transform_flags);
}

void Editor::CreatePointLightMenu(const char* title, PointLight* point_light)
{
    Get().CreatePointLightMenuImpl(title, point_light);
}

void Editor::CreateDirectionalLightMenu(const char* title, DirectionalLight* directional_light)
{
    Get().CreateDirectionalLightMenuImpl(title, directional_light);
}

void Editor::CreateFiltersMenu(const char* title, Shader& shader, float time)
{
    Get().CreateFiltersMenuImpl(title, shader, time);
}

void Editor::CreateBackgroundMenu(const char* title, glm::vec3& color)
{
    Get().CreateBackgroundMenuImpl(title, color);
}

void Editor::CreateTextInput(const char* title, Model* model)
{
    Get().CreateTextInputImpl(title, model);
}

bool Editor::MouseIsOnEditor()
{
    return Get().MouseIsOnEditorImpl();
}

bool Editor::ShowSkybox()
{
    return Get().ShowSkyboxImpl();
}

Editor::~Editor()
{
}

Editor::Editor() :
    m_filter_type{0}, m_vignette_intensity{1.0f}, m_blur_intensity{0.07f}, m_show_skybox{true}
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
    //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
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

void Editor::CreateTransformMenuImpl(const char* title, Transform *transform, uint8_t transform_flags)
{
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
}

void Editor::CreatePointLightMenuImpl(const char* title, PointLight *point_light)
{
    ImGui::Begin(title);
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
    CreateTransformMenuImpl(title, &(point_light->GetModel().GetTransform()), TRANSFORM_TRANSLATION | TRANSFORM_ROTATION | TRANSFORM_SCALE | TRANSFORM_SCALE_UNIFORM);
    point_light->UpdateColors();
    ImGui::End();
}

void Editor::CreateDirectionalLightMenuImpl(const char* title, DirectionalLight* directional_light)
{
    ImGui::Begin(title);
    if (ImGui::Button("On/Off"))
    {
        directional_light->Toggle();
    }

    ImGui::DragFloat3("Direction", &(directional_light->GetDirection().x), 0.1f, -100.0f, 100.0f);
    ImGui::End();
}

void Editor::CreateFiltersMenuImpl(const char* title, Shader& shader, float time)
{
    ImGui::Begin(title);
    ImGui::RadioButton("Normal", &Get().m_filter_type, 0);
    ImGui::RadioButton("Inverted", &Get().m_filter_type, 1);
    ImGui::RadioButton("Greyscale", &Get().m_filter_type, 2);
    ImGui::RadioButton("Vignette", &Get().m_filter_type, 3);
    ImGui::DragFloat("Vignette Intensity", &Get().m_vignette_intensity, 0.1f, 0.0f, 2.0f);
    ImGui::RadioButton("Blur Sweep", &Get().m_filter_type, 4);
    ImGui::DragFloat("Blur Intensity", &Get().m_blur_intensity, 0.001f, 0.0f, 0.07f);
    ImGui::End();
    shader.SetUniform1i("filter_type", Get().m_filter_type);
    shader.SetUniform1f("vignette_intensity", Get().m_vignette_intensity);
    shader.SetUniform1f("time", time);
    shader.SetUniform1f("blur_intensity", Get().m_blur_intensity);
}

void Editor::CreateBackgroundMenuImpl(const char* title, glm::vec3& color)
{
    ImGui::Begin(title);
    if (ImGui::Button("Show Skybox"))
    {
        Get().ToggleSkybox();
    }
    ImGui::ColorEdit3("Color", (float*)&(color));
    ImGui::End();
}

void Editor::CreateTextInputImpl(const char* title, Model *model)
{
    ImGui::Begin(title);
    ImGui::InputText(title, model_input_buffer, 160);

    if (ImGui::Button("Load model"))
    {
        model->LoadNewModel(std::string(model_input_buffer));
    }

    ImGui::End();
}

bool Editor::MouseIsOnEditorImpl()
{
    return ImGui::GetIO().WantCaptureMouse;
}

bool Editor::ShowSkyboxImpl()
{
    return Get().m_show_skybox;
}

void Editor::ToggleSkybox()
{
    Get().m_show_skybox = 1 - (Get().m_show_skybox);
}
