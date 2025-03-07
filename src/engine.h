#pragma once

#include <memory>
#include <string>
#include <vector>

#include "raylib.h"

enum PropellantType
{
    METHALOX = 0,
    HYDROLOX,
    KEROSENE,
    SOLID_FUEL,
    HYPERGOLIC,
    END
};

constexpr char* PROP_NAMES[] = {
    "Liquid Methane",
    "Liquid Hydrogen",
    "Kerosene (RP-1)",
    "Solid Propellant",
    "Hypergolic"
};

struct EngineConfiguration
{
    float throatDiameter = 1.0f;
    float nozzleDiameter = 1.0f;
    float nozzleLength = 1.0f;
    float nozzleCurve = 0.5f;
    float chamberDiameter = 1.0f;
    float chamberLength = 2.0f;
    PropellantType propellant = PropellantType::METHALOX;

    EngineConfiguration(float throatDiameter, float nozzleDiameter, float nozzleLength, float nozzleCurve, float chamberDiameter, float chamberLength, PropellantType prop)
        :   throatDiameter(throatDiameter), nozzleDiameter(nozzleDiameter), nozzleLength(nozzleLength),
            nozzleCurve(nozzleCurve), chamberDiameter(chamberDiameter), chamberLength(chamberLength), propellant(prop)
    {

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
