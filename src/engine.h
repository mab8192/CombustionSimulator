#pragma once

#include <memory>
#include <string>
#include <vector>

enum PropellantType
{
    METHALOX = 0,
    HYDROLOX,
    KEROSENE
};

struct EngineConfiguration
{
    float throatDiameter = 1.0f;
    float nozzleDiameter = 1.0f;
    float nozzleLength = 1.0f;
    float nozzleCurve = 0.0f;
    float chamberVolume = 1.0f;
    float chamberLength = 2.0f;
    PropellantType propellant = PropellantType::METHALOX;
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

    /* Calculates geometry and stores results in m_Vertices to be used for rendering */
    void CalculateGeometry();
    const std::vector<float> &GetVertices() const;

private:
    std::shared_ptr<EngineConfiguration> m_Config;

    // Computed values
    float m_Thrust = 0.0f;
    float m_ChamberPressure = 0.0f;

    // Rendering
    std::vector<float> m_Vertices;
};
