#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

// Ошибки GLFW выводим в консоль
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Вкладки меню
enum class Tab
{
    Main,
    Visuals,
    Misc,
    Settings
};

// Настройка цветов и скругления
static void SetupStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 4.0f;
    style.FrameRounding  = 4.0f;
    style.ScrollbarRounding = 6.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]         = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    colors[ImGuiCol_Header]           = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderHovered]    = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_HeaderActive]     = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Button]           = colors[ImGuiCol_Header];
    colors[ImGuiCol_ButtonHovered]    = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_ButtonActive]     = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_FrameBg]          = colors[ImGuiCol_Header];
    colors[ImGuiCol_FrameBgHovered]   = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_FrameBgActive]    = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_CheckMark]        = ImVec4(0.90f, 0.50f, 0.30f, 1.00f);
    colors[ImGuiCol_SliderGrab]       = ImVec4(0.90f, 0.50f, 0.30f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.40f, 0.20f, 1.00f);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(800, 600, "PastOwl Menu", nullptr, nullptr);
    if (!window)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SetupStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    bool show_menu = true;
    bool show_demo = false;
    bool some_option = false;
    bool visuals_box = false;
    float strength = 50.0f;
    ImVec4 color = ImVec4(0.9f, 0.5f, 0.3f, 1.0f);
    Tab current = Tab::Main;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Insert))
            show_menu = !show_menu;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_menu)
        {
            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
            ImGui::Begin("PastOwl Menu", &show_menu, ImGuiWindowFlags_NoCollapse);

            ImGui::BeginGroup();
            if (ImGui::Button("\u0413\u043B\u0430\u0432\u043D\u0430\u044F", ImVec2(120, 0))) current = Tab::Main;
            if (ImGui::Button("\u0412\u0438\u0437\u0443\u0430\u043B\u044B", ImVec2(120, 0))) current = Tab::Visuals;
            if (ImGui::Button("\u0420\u0430\u0437\u043D\u043E\u0435", ImVec2(120, 0))) current = Tab::Misc;
            if (ImGui::Button("\u041D\u0430\u0441\u0442\u0440\u043E\u0439\u043A\u0438", ImVec2(120, 0))) current = Tab::Settings;
            ImGui::EndGroup();
            ImGui::SameLine();

            ImGui::BeginGroup();
            switch (current)
            {
                case Tab::Main:
                    ImGui::Checkbox("\u041E\u043F\u0446\u0438\u044F", &some_option);
                    ImGui::SliderFloat("\u0421\u0438\u043B\u0430", &strength, 0.0f, 100.0f);
                    break;
                case Tab::Visuals:
                    ImGui::Checkbox("\u0411\u043E\u043A\u0441", &visuals_box);
                    ImGui::ColorEdit3("\u0426\u0432\u0435\u0442", (float*)&color);
                    break;
                case Tab::Misc:
                    ImGui::Text("\u0414\u0440\u0443\u0433\u0438\u0435 \u0444\u0443\u043D\u043A\u0446\u0438\u0438");
                    break;
                case Tab::Settings:
                    ImGui::Checkbox("\u041F\u043E\u043A\u0430\u0437\u044B\u0432\u0430\u0442\u044C demo", &show_demo);
                    break;
            }
            ImGui::EndGroup();

            ImGui::End();
        }

        if (show_demo)
            ImGui::ShowDemoWindow(&show_demo);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
