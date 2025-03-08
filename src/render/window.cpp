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

void Window::DrawEngineCrossSection() const
{
    // Access engine configuration
    auto config = m_Engine->GetConfiguration();
    float throatDiameter = config->throatDiameter;
    float nozzleDiameter = config->nozzleDiameter;
    float nozzleLength = config->nozzleLength;
    float chamberDiameter = config->chamberDiameter;
    float chamberLength = config->chamberLength;
    const EngineNozzle& nozzle = config->nozzle;

    // Compute radii
    float R_throat = throatDiameter / 2.0f;
    float R_chamber = chamberDiameter / 2.0f;
    float R_exit = nozzleDiameter / 2.0f;

    // Convergent section: circular arc
    float R_conv = nozzle.entranceRadius * R_throat; // Radius of curvature at throat entrance
    float diff = R_throat - R_chamber + R_conv;
    float L_conv = std::sqrt(R_conv * R_conv - diff * diff); // Length of convergent section
    float c_x = -L_conv; // x-coordinate of arc center
    float c_y = R_chamber - R_conv; // y-coordinate of arc center
    float theta_start = PI / 2.0f; // Starting angle (vertical up)
    float theta_end = std::atan2(R_throat - c_y, 0 - c_x); // Ending angle at throat

    // Divergent section: parabolic curve y(x) = R_throat + a*x + b*x^2
    float a = std::tan(nozzle.inflectionAngle); // Initial slope at throat
    float b = (std::tan(nozzle.exitAngle) - a) / (2.0f * nozzleLength); // Coefficient for curvature

    // Generate upper contour points
    std::vector<Vector2> upper_contour;

    // 1. Cylindrical chamber: straight line from x = -chamberLength to x = -L_conv
    upper_contour.push_back({ -chamberLength, R_chamber });
    upper_contour.push_back({ -L_conv, R_chamber });

    // 2. Convergent section: circular arc from x = -L_conv to x = 0
    int num_conv_points = 20;
    float dtheta = (theta_end - theta_start) / num_conv_points;
    for (int i = 1; i <= num_conv_points; i++) {
        float theta = theta_start + i * dtheta;
        float x = c_x + R_conv * std::cos(theta);
        float y = c_y + R_conv * std::sin(theta);
        upper_contour.push_back({ x, y });
    }

    // 3. Divergent section: parabolic curve from x = 0 to x = nozzleLength
    int num_div_points = 50;
    float dx = nozzleLength / num_div_points;
    for (int i = 0; i <= num_div_points; i++) {
        float x = i * dx;
        float y = R_throat + a * x + b * x * x;
        upper_contour.push_back({ x, y });
    }

    // Generate lower contour by mirroring across y = 0
    std::vector<Vector2> lower_contour;
    for (const auto& p : upper_contour) {
        lower_contour.push_back({ p.x, -p.y });
    }

    // Compute scaling factor to fit the screen
    float total_length = chamberLength + nozzleLength;
    float margin = 50.0f;
    float s_x = (GetScreenWidth() - 2 * margin) / total_length;
    float s_y = (GetScreenHeight() / 2.0f - margin) / R_chamber;
    float scale = std::min(s_x, s_y);

    // Transform points to screen coordinates
    for (auto& p : upper_contour) {
        p.x = GetScreenWidth() / 2.0f + scale * p.x;
        p.y = GetScreenHeight() / 2.0f - scale * p.y;
    }
    for (auto& p : lower_contour) {
        p.x = GetScreenWidth() / 2.0f + scale * p.x;
        p.y = GetScreenHeight() / 2.0f - scale * p.y;
    }

    // Draw upper and lower contours
    DrawLineStrip(upper_contour.data(), static_cast<int>(upper_contour.size()), BLACK);
    DrawLineStrip(lower_contour.data(), static_cast<int>(lower_contour.size()), BLACK);

    // Optional: Draw centerline
    DrawLine(
        GetScreenWidth() / 2.0f + scale * (-chamberLength), GetScreenHeight() / 2.0f,
        GetScreenWidth() / 2.0f + scale * nozzleLength, GetScreenHeight() / 2.0f,
        GRAY
    );
}

void Window::Draw() const
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    DrawEngineCrossSection();

    // start ImGui Conent
    rlImGuiBegin();

    // show ImGui Content
    if (ImGui::Begin("Engine Configuration"))
    {
        // Basic engine parameters
        ImGui::SliderFloat("Chamber Diameter", &(m_Engine->GetConfiguration()->chamberDiameter), 0.1f, 10.0f);
        ImGui::SliderFloat("Chamber Length", &(m_Engine->GetConfiguration()->chamberLength), 0.1f, 10.0f);
        ImGui::SliderFloat("Throat Diameter", &(m_Engine->GetConfiguration()->throatDiameter), 0.1f, 10.0f);
        ImGui::SliderFloat("Nozzle Diameter", &(m_Engine->GetConfiguration()->nozzleDiameter), 0.1f, 10.0f);
        ImGui::SliderFloat("Nozzle Length", &(m_Engine->GetConfiguration()->nozzleLength), 0.1f, 10.0f);

        if (ImGui::CollapsingHeader("Nozzle Profile (Rao Parameters)"))
        {
            ImGui::Indent();

            ImGui::SliderFloat("Entrance Radius", &(m_Engine->GetConfiguration()->nozzle.entranceRadius),
                0.5f, 3.0f, "%.2f x throat radius");
            ImGui::SliderFloat("Throat Radius", &(m_Engine->GetConfiguration()->nozzle.throatRadius),
                0.2f, 1.0f, "%.2f x throat radius");
            ImGui::SliderFloat("Divergent Radius", &(m_Engine->GetConfiguration()->nozzle.divergentRadius),
                0.3f, 1.5f, "%.2f x throat radius");

            float inflectionAngleDegrees = m_Engine->GetConfiguration()->nozzle.inflectionAngle * 180.0f / PI; // Convert radians to degrees
            if (ImGui::SliderFloat("Inflection Angle", &inflectionAngleDegrees, 20.0f, 60.0f, "%.1f deg"))
            {
                m_Engine->GetConfiguration()->nozzle.inflectionAngle = inflectionAngleDegrees * PI / 180.0f; // Convert degrees to radians
            }

            float exitAngleDegrees = m_Engine->GetConfiguration()->nozzle.exitAngle * 180.0f / PI; // Convert radians to degrees
            if (ImGui::SliderFloat("Exit Angle", &exitAngleDegrees, 5.0f, 20.0f, "%.1f deg"))
            {
                m_Engine->GetConfiguration()->nozzle.exitAngle = exitAngleDegrees * PI / 180.0f; // Convert degrees to radians
            }

            // Show a preview of the expansion ratio resulting from these parameters
            float expansionRatio = m_Engine->GetConfiguration()->getExpansionRatio();
            ImGui::Text("Expansion Ratio: %.2f", expansionRatio);

            // Add a button to reset to optimal Rao parameters
            if (ImGui::Button("Reset to Optimal Rao Parameters"))
            {
                m_Engine->GetConfiguration()->nozzle.entranceRadius = 1.5f;
                m_Engine->GetConfiguration()->nozzle.throatRadius = 0.4f;
                m_Engine->GetConfiguration()->nozzle.divergentRadius = 0.5f;
                m_Engine->GetConfiguration()->nozzle.inflectionAngle = 0.785398f; // 45 degrees
                m_Engine->GetConfiguration()->nozzle.exitAngle = 0.261799f; // 15 degrees
            }

            ImGui::Unindent();
        }

        // Array of rocket fuel types
        // Get the current propellant type from the engine configuration
        const Propellant& currentPropellant = m_Engine->GetConfiguration()->propellant;

        // Create the combo box
        if (ImGui::BeginCombo("Rocket Fuel Type", currentPropellant.name)) {
            for (int i = 0; i < IM_ARRAYSIZE(ALL_PROPS); i++) {
                bool isSelected = (strcmp(currentPropellant.name, ALL_PROPS[i].name) == 0);
                if (ImGui::Selectable(ALL_PROPS[i].name, isSelected)) {
                    m_Engine->GetConfiguration()->propellant = ALL_PROPS[i];
                }
                if (isSelected) {
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
