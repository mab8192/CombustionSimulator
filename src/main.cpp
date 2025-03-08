#include "render/window.h"

#include <iostream>
#include <memory>

int main()
{
    Window window;
    window.Init(1600, 900, "Rocket Simulation Visual Studio");

    std::shared_ptr<EngineConfiguration> config = std::make_shared<EngineConfiguration>();
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
