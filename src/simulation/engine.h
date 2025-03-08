#pragma once

#include <memory>
#include <string>
#include <vector>

#include "raylib.h"

#include "Propellant.h"


struct EngineNozzle
{
    float entranceRadius = 1.5f;     // Radius of curvature at throat entrance (multiple of throat radius)
    float throatRadius = 0.4f;       // Radius of curvature at throat (multiple of throat radius)
    float divergentRadius = 0.5f;    // Initial divergent section radius of curvature
    float inflectionAngle = 0.785f;  // Inflection angle in radians (typically ~45°)
    float exitAngle = 0.262f;        // Exit angle in radians (typically ~15°)
};


struct EngineConfiguration
{
    float throatDiameter = 1.0f;    // m
    float nozzleDiameter = 1.0f;    // m
    float nozzleLength = 1.0f;      // m
    float chamberDiameter = 1.0f;   // m
    float chamberLength = 2.0f;     // m

    EngineNozzle nozzle;

    Propellant propellant = Propellants::KEROSENE;

    EngineConfiguration() = default;

    EngineConfiguration(float throatDiameter, float nozzleDiameter, float nozzleLength, float chamberDiameter, float chamberLength, const EngineNozzle& nozzle, const Propellant& prop)
        :  
        throatDiameter(throatDiameter),
        nozzleDiameter(nozzleDiameter), 
        nozzleLength(nozzleLength),
        chamberDiameter(chamberDiameter), 
        chamberLength(chamberLength), 
        nozzle(nozzle),
        propellant(prop)
    { }

    // Calculate expansion ratio (often needed in rocket calculations)
    float getExpansionRatio() const {
        return (nozzleDiameter * nozzleDiameter) / (throatDiameter * throatDiameter);
    }

    // Calculate chamber volume
    float getChamberVolume() const {
        const float radius = chamberDiameter / 2.0f;
        return PI * radius * radius * chamberLength;
    }
};

class Engine
{
public:
    Engine() = default;
    Engine(std::shared_ptr<EngineConfiguration> config);

    std::shared_ptr<EngineConfiguration> GetConfiguration() const;
    void UpdateConfiguration(std::shared_ptr<EngineConfiguration> config); // pass by value
    float ComputeThrust() const;
    float ComputeExhaustVelocity() const;

private:
    std::shared_ptr<EngineConfiguration> m_Config;

    // Computed values
    float m_Thrust = 0.0f;
    float m_ChamberPressure = 0.0f;
};
