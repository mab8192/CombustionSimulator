#include "ui.h"
#include <imgui.h>

void drawUI(Configuration& config, const Engine& simulation) {
    // Configuration window
    ImGui::Begin("Engine Configuration");
    ImGui::PushItemWidth(100);
    static int fuel_index = 0;
    const char* fuels[] = {"Hydrogen", "Kerosene", "Methane", "Solid Fuel"};
    if (ImGui::Combo("Fuel Type", &fuel_index, fuels, IM_ARRAYSIZE(fuels))) {
        config.fuel_type = fuels[fuel_index];
    }
    ImGui::SliderFloat("Nozzle Diameter (m)", &config.nozzle_diameter, 0.1f, 10.0f);
    ImGui::End();

    // Results window
    ImGui::Begin("Simulation Results");
    ImGui::Text("Thrust: %.2f N", simulation.getThrust());
    // Add more results as simulation expands
    ImGui::End();
}
