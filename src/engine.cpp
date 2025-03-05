#include "engine.h"

void Engine::update(const Configuration& config) {
    // Simplified thrust calculation (example only)
    if (config.fuel_type == "Hydrogen") {
        thrust = 1000.0f * config.nozzle_diameter;  // Arbitrary scaling
    } else if (config.fuel_type == "Kerosene") {
        thrust = 800.0f * config.nozzle_diameter;
    } else if (config.fuel_type == "Methane") {
        thrust = 900.0f * config.nozzle_diameter;
    }
    // Extend with real physics later (e.g., mass flow rate, specific impulse)
}
