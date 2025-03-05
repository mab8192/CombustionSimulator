#pragma once

#include <string>

class Configuration {
public:
    std::string fuel_type = "Hydrogen";  // Default fuel type
    float nozzle_diameter = 1.0f;        // Default nozzle diameter (meters)
    // Add more parameters as needed (e.g., chamber pressure, shape)
};
