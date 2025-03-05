#pragma once

#include "config.h"

class Engine {
public:
    Engine() : thrust(0.0f) {}
    void update(const Configuration& config);
    float getThrust() const { return thrust; }
    // Add more getters for other results (e.g., exhaust velocity)

private:
    float thrust;  // Simulation output
    // Add more state variables as simulation complexity increases
};
