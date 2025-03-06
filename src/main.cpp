#include "render/renderer.h"

#include <memory>

int main()
{
    Renderer renderer;
    renderer.Init(1600, 900, "Rocket Simulation");

    std::shared_ptr<EngineConfiguration> config = std::make_shared<EngineConfiguration>(
        0.5f, // throat diameter
        1.0f, // nozzle diameter
        1.0f, // nozzle length
        0.0f, // nozzle curve
        2.0f, // chamber volume
        2.0f, // chamber length
        PropellantType::METHALOX);

    Engine engine(config);
    engine.CalculateGeometry();

    while (!renderer.ShouldClose())
    {
        glfwPollEvents();

        engine.CalculateGeometry();

        renderer.BeginRender();
        renderer.DrawEngine(engine);

        // ImGui UI
        ImGui::Begin("Engine Configuration");
        ImGui::SliderFloat("Throat Diameter", &engine.GetConfiguration()->throatDiameter, 0.1f, 4.0f);
        ImGui::SliderFloat("Nozzle Diameter", &engine.GetConfiguration()->nozzleDiameter, 0.1f, 4.0f);
        ImGui::SliderFloat("Chamber Volume", &engine.GetConfiguration()->chamberVolume, 0.1f, 4.0f);
        ImGui::SliderFloat("Chamber Length", &engine.GetConfiguration()->chamberLength, 0.1f, 4.0f);
        ImGui::End();

        renderer.DrawImGui();

        glfwSwapBuffers(renderer.GetWindow());
    }

    renderer.Shutdown();
}