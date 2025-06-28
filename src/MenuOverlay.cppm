module;
#include <vulkan/vulkan.hpp>
#include "imgui.h"
#include <charconv>
export module MenuOverlay;
import obscure.imgui;
import obscure.utils.stopwatch;
import GlobalState;

#pragma region application
bool should_exit = false;
export [[nodiscard]] bool MenuExit() noexcept
{
    return should_exit;
}

#pragma endregion

#pragma region framerate
bool showFramerate = false;
obscure::stopwatch<> stopwatch{};
obscure::stopwatch<>::duration_t frame_time;
#pragma endregion

#pragma region controls
bool showControls = true;
constexpr const char controlsText[] =
R"(Controls   :
    Exit            -> [ESC]
    Switch Camera   -> [TAB]
Vehicle Controls:
    Animate 2 Drones    -> R
    Animate Vehicles    -> T
Camera Controls:
    Forward     -> W
    Backward    -> S
    Left        -> A
    Right       -> D
    Up          -> [SPACE]
    Down        -> [LEFT SHIFT]
Free Camera Controls:
    Pan/Tilt    -> [MOUSE CURSOR]
Top Down Camera Controls:
    Rotate Right    -> E
    Rotate Left     -> Q)";
#pragma endregion

#pragma region camera mode
bool show_camera_mode = false;
const char * getModeText() {
    if (global::CurrentCameraMode() == global::CameraMode::Free) {
        return "Free     Camera";
    }
    else {
        return "Top Down Camera";
    }
}
#pragma endregion
void drawOverlay()
{
    static bool overlayOpen = false;
    constexpr static float PAD = 10.0f;
    constexpr static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
    if (showFramerate || show_camera_mode || showControls)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = work_pos.x + work_size.x - PAD;
        window_pos.y = work_pos.y + PAD;
        window_pos_pivot.x = 1.0f;
        window_pos_pivot.y = 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowBgAlpha(0.35f);

        if (ImGui::Begin("Overlay", &overlayOpen, window_flags))
        {
            if (showFramerate)
            {
                char frameRateText[] = "Frame Rate : 0000";
                char* number_start = frameRateText + 13;
                std::to_chars(frameRateText + 13, frameRateText + 17, static_cast<uint32_t>(1.0 / frame_time.count()));
                ImGui::Text(frameRateText);
            }
            if (show_camera_mode) {
                ImGui::Text(getModeText());
            }
            if (showControls)
            {
                ImGui::Text(controlsText);
            }

            ImGui::End();
        }
    }
}

void drawMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Application"))
        {
            if (ImGui::MenuItem("Exit", "ESC"))
            {
                should_exit = true;
            }
            if (ImGui::MenuItem("Toggle Camera Mode", "[TAB]")) {
                global::toggleCameraMode();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Overlay"))
        {
            ImGui::MenuItem("Show Framerate", "", &showFramerate);
            ImGui::MenuItem("Show Camera Mode", "", &show_camera_mode);
            ImGui::MenuItem("Show Controls", "", &showControls);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Vehicles")) {
            ImGui::MenuItem("Toggle Drones", "R", &global::AnimateFirst2Drones());
            ImGui::MenuItem("Toggle Cars", "T", &global::AnimateCars());
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

export void drawMenuOverlay(vk::CommandBuffer command_buffer)
{
    obscure::imgui::frame imgui_frame{command_buffer};
    frame_time = stopwatch.lap_time();
    drawMenu();
    drawOverlay();
    ImGui::ShowDemoWindow();
}