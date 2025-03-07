#include "render/window.h"

#include <iostream>
#include <memory>

int main()
{
    Window window;
    window.Init(1600, 900, "Rocket Simulation");

    std::shared_ptr<EngineConfiguration> config = std::make_shared<EngineConfiguration>(
        0.5f, // throat diameter
        1.0f, // nozzle diameter
        1.0f, // nozzle length
        0.0f, // nozzle curve
        2.0f, // chamber volume
        2.0f, // chamber length
        PropellantType::METHALOX);

    std::shared_ptr<Engine> engine = std::make_shared<Engine>(config);
    window.SetEngine(engine);

    while (!window.ShouldClose())
    {
        // TODO Update simulation

        window.Draw();
    }

    std::cout << "Closing!" << std::endl;
    window.Close();
}
