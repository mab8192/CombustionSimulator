#include "window.h"

#include "raylib.h"
#include "imgui.h"
#include "rlImGui.h"

#include <iostream>

Window::Window() {}

Window::~Window()
{
    Close();
}

void Window::Init(int width, int height, const std::string &title)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(width, height, title.c_str());

    SetTargetFPS(120);

    // Init ImGui
    rlImGuiSetup(true);
}

void Window::SetEngine(std::shared_ptr<Engine> engine)
{
    m_Engine = engine;
}

void Window::Draw()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    // TODO: Draw engine


    // start ImGui Conent
    rlImGuiBegin();

    // show ImGui Content
    if (ImGui::Begin("Engine Configuration"))
    {
        ImGui::SliderFloat("Chamber Diameter", &(m_Engine->GetConfiguration()->chamberDiameter), 0.1f, 10.0f);
        ImGui::SliderFloat("Chamber Length", &(m_Engine->GetConfiguration()->chamberLength), 0.1f, 10.0f);
        ImGui::SliderFloat("Throat Diameter", &(m_Engine->GetConfiguration()->throatDiameter), 0.1f, 10.0f);
        ImGui::SliderFloat("Nozzle Diameter", &(m_Engine->GetConfiguration()->nozzleDiameter), 0.1f, 10.0f);
        ImGui::SliderFloat("Nozzle Length", &(m_Engine->GetConfiguration()->nozzleLength), 0.1f, 10.0f);
        ImGui::SliderFloat("Nozzle Curve", &(m_Engine->GetConfiguration()->nozzleCurve), 0.0f, 1.0f);

        // Array of rocket fuel types
        // Get the current propellant type from the engine configuration
        PropellantType& current_propellant = m_Engine->GetConfiguration()->propellant;
        int current_index = static_cast<int>(current_propellant); // Convert enum to int for ImGui

        static_assert(IM_ARRAYSIZE(PROP_NAMES) == static_cast<int>(PropellantType::END),
                      "PROP_NAMES array size must match size of PropellantType enum");

        // Create the combo box
        if (ImGui::BeginCombo("Rocket Fuel Type", PROP_NAMES[current_index])) {
            for (int i = 0; i < IM_ARRAYSIZE(PROP_NAMES); i++) {
                bool is_selected = (current_index == i);
                if (ImGui::Selectable(PROP_NAMES[i], is_selected)) {
                    current_propellant = static_cast<PropellantType>(i); // Update the enum value
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus(); // Focus on the current selection
                }
            }
            ImGui::EndCombo();
        }
        ImGui::End();
    }

    if (ImGui::Begin("Simulation"))
    {
        if (ImGui::Button("Restart Simulation"))
        {
            std::cout << "Button Pressed!" << std::endl;
        }

        ImGui::End();
    }

    // end ImGui Content
    rlImGuiEnd();

    DrawFPS(10, 10);
    EndDrawing();
}

bool Window::ShouldClose() const
{
    return WindowShouldClose();
}

void Window::Close()
{
    if (m_Shutdown) return;

    m_Shutdown = true;

    rlImGuiShutdown();
    CloseWindow();
}
